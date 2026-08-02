#!/usr/bin/env python3
"""
FnCoder84 Raw HID encoder monitor (PCoIP / local path test).

Protocol: keyboards/fncoder84/HOST_RAW_HID_PROTOCOL.md

  0xFE  host → KB  LEDs
  0xFC  host → KB  STREAM / PING
  0xFD  KB → host  TURN / BUTTON / PONG

Dependencies (macOS — verified):

  # Prefer the Cython package named "hidapi" (provides import hid).
  # Do NOT install the separate ctypes package named "hid" — it shadows
  # import hid and fails unless libhidapi.dylib is on the loader path.
  # (QMK's pip deps sometimes install "hid"; uninstall it if import breaks.)
  #
  #   brew install hidapi          # optional native lib for other tools
  #   python3 -m pip uninstall -y hid hidapi
  #   python3 -m pip install --user hidapi
  #
  # Raw HID interface: usage_page 0xFF60 (this script prefers that).

Usage:
  python3 raw_hid_encoder_monitor.py
  python3 raw_hid_encoder_monitor.py --vid 0xAF84 --pid 0x0084
"""

from __future__ import annotations

import argparse
import sys
import time

try:
    import hid
except ImportError as e:
    print(
        "Could not import hid.\n"
        "  python3 -m pip uninstall -y hid hidapi\n"
        "  python3 -m pip install --user hidapi\n"
        f"({e})",
        file=sys.stderr,
    )
    sys.exit(1)

VID_DEFAULT = 0xAF84
PID_DEFAULT = 0x0084
REPORT_LEN = 32

FNCODER_CTRL = 0xFC
FNCODER_EVT = 0xFD
CTRL_STREAM = 0x01
CTRL_PING = 0x02
EVT_TURN = 0x01
EVT_BUTTON = 0x02
EVT_PONG = 0x7F


def i8(b: int) -> int:
    return b if b < 128 else b - 256


def open_device(vid: int, pid: int) -> hid.device:
    """Open the Raw HID interface if possible; fall back to first matching path."""
    targets = []
    for info in hid.enumerate(vid, pid):
        targets.append(info)
        print(
            f"  found: path={info.get('path')!r} "
            f"usage_page=0x{info.get('usage_page', 0):04X} "
            f"usage=0x{info.get('usage', 0):04X} "
            f"interface={info.get('interface_number')}"
        )

    if not targets:
        raise SystemExit(f"No HID device {vid:04X}:{pid:04X}. Is the board plugged in?")

    # Prefer interfaces that look like vendor/raw (not boot keyboard).
    # QMK Raw HID is often usage_page 0xFF60.
    preferred = [t for t in targets if t.get("usage_page") == 0xFF60]
    choice = preferred[0] if preferred else targets[0]

    dev = hid.device()
    dev.open_path(choice["path"])
    dev.set_nonblocking(True)
    print(f"Opened path={choice.get('path')!r}")
    return dev


def write_report(dev: hid.device, payload: bytes) -> None:
    """Send a 32-byte report; Windows often wants a leading report-id 0."""
    if len(payload) != REPORT_LEN:
        raise ValueError("payload must be 32 bytes")
    try:
        dev.write(b"\x00" + payload)
    except Exception:
        dev.write(payload)


def read_report(dev: hid.device, timeout_s: float = 0.0) -> bytes | None:
    deadline = time.time() + timeout_s
    while True:
        data = dev.read(REPORT_LEN + 1)  # allow report-id byte
        if data:
            raw = bytes(data)
            # Strip leading report id if present and length is 33
            if len(raw) == REPORT_LEN + 1 and raw[0] == 0:
                raw = raw[1:]
            if len(raw) >= REPORT_LEN:
                return raw[:REPORT_LEN]
            if len(raw) > 0:
                return raw.ljust(REPORT_LEN, b"\x00")
        if timeout_s <= 0 or time.time() >= deadline:
            return None
        time.sleep(0.005)


def main() -> int:
    ap = argparse.ArgumentParser(description="FnCoder84 Raw HID encoder monitor")
    ap.add_argument("--vid", type=lambda s: int(s, 0), default=VID_DEFAULT)
    ap.add_argument("--pid", type=lambda s: int(s, 0), default=PID_DEFAULT)
    ap.add_argument("--no-ping", action="store_true", help="Skip PING/PONG path check")
    ap.add_argument("--stream-off", action="store_true", help="Send STREAM off then exit")
    args = ap.parse_args()

    print(f"Looking for {args.vid:04X}:{args.pid:04X} ...")
    dev = open_device(args.vid, args.pid)

    if args.stream_off:
        pkt = bytes([FNCODER_CTRL, CTRL_STREAM, 0] + [0] * 29)
        write_report(dev, pkt)
        print("Sent STREAM off")
        dev.close()
        return 0

    # Ensure stream on
    write_report(dev, bytes([FNCODER_CTRL, CTRL_STREAM, 1] + [0] * 29))
    print("Sent STREAM on")

    if not args.no_ping:
        cookie = 0xA5
        write_report(dev, bytes([FNCODER_CTRL, CTRL_PING, cookie] + [0] * 29))
        print(f"Sent PING cookie=0x{cookie:02X}, waiting for PONG...")
        t0 = time.time()
        got = False
        while time.time() - t0 < 2.0:
            rep = read_report(dev, timeout_s=0.05)
            if not rep:
                continue
            if rep[0] == FNCODER_EVT and rep[1] == EVT_PONG and rep[3] == cookie:
                rtt_ms = (time.time() - t0) * 1000
                print(f"PONG ok  seq={rep[4]}  rtt≈{rtt_ms:.1f} ms  — HID path works")
                got = True
                break
            # Print other events that arrive while waiting
            if rep[0] == FNCODER_EVT:
                print(f"  (event while waiting) {rep[:6].hex(' ')}")
        if not got:
            print(
                "No PONG — wrong interface, OS filter, or PCoIP not forwarding Raw HID IN.",
                file=sys.stderr,
            )

    values = [0] * 12
    buttons = [0] * 12
    print("Listening for TURN/BUTTON (Ctrl+C to stop)...")
    print("  TURN: value[i] += delta    BUTTON: press=1 release=0")

    try:
        while True:
            rep = read_report(dev, timeout_s=0.1)
            if not rep:
                continue
            if rep[0] != FNCODER_EVT:
                continue
            msg, idx, payload, seq = rep[1], rep[2], rep[3], rep[4]
            if msg == EVT_TURN and idx < 12:
                d = i8(payload)
                values[idx] += d
                print(f"TURN  enc={idx:2d}  delta={d:+d}  value={values[idx]:6d}  seq={seq}")
            elif msg == EVT_BUTTON and idx < 12:
                buttons[idx] = payload
                state = "DOWN" if payload else "UP"
                print(f"BUTTON enc={idx:2d}  {state}  seq={seq}")
            elif msg == EVT_PONG:
                print(f"PONG  cookie=0x{payload:02X}  seq={seq}")
            else:
                print(f"EVT   type=0x{msg:02X} idx={idx} payload=0x{payload:02X} seq={seq}")
    except KeyboardInterrupt:
        print("\nBye")
    finally:
        dev.close()
    return 0


if __name__ == "__main__":
    sys.exit(main())
