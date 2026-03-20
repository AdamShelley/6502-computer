import serial
import sys
import time

PORT = "/dev/cu.usbserial-A5069RR4"  # change to your port, e.g. COM3 on Windows
BAUD = 57600

def flash(bin_path):
    with open(bin_path, "rb") as f:
        rom = f.read()

    with serial.Serial(PORT, BAUD, timeout=5) as ser:
        time.sleep(2)  # wait for Arduino reset
        
        # Wait for READY
        response = ser.readline().decode().strip()
        if response != "READY":
            print(f"Unexpected response: {response}")
            sys.exit(1)
        print("Programmer ready")

        for address, byte in enumerate(rom):
            addr_high = (address >> 8) & 0xff
            addr_low  = address & 0xff
            ser.write(bytes([addr_high, addr_low, byte]))

            ack = ser.read(1)
            if ack != b'K':
                print(f"No ACK at address {address:#06x}, got {ack}")
                sys.exit(1)

            if address % 256 == 0:
                print(f"  {address:#06x} / {len(rom):#06x}")

        print("Done!")

if __name__ == "__main__":
    flash(sys.argv[1])