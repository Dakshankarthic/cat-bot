"""
Cat Bot Python Controller
-------------------------
Control your Cat Bot over Wi-Fi (REST API) or Serial.
"""
import requests
import time
import sys

DEFAULT_ROBOT_URL = "http://192.168.4.1"  # Default IP when connected to Cat Bot Wi-Fi AP
FALLBACK_MDNS_URL = "http://sesame-robot.local"

def get_base_url():
    # Try local AP IP first, then fallback to mDNS
    for url in [DEFAULT_ROBOT_URL, FALLBACK_MDNS_URL]:
        try:
            r = requests.get(f"{url}/api/status", timeout=1.5)
            if r.status_code == 200:
                return url
        except Exception:
            pass
    return DEFAULT_ROBOT_URL

def send_command(url, command=None, face=None):
    payload = {}
    if command:
        payload["command"] = command
    if face:
        payload["face"] = face
    
    try:
        r = requests.post(f"{url}/api/command", json=payload, timeout=3)
        return r.json()
    except Exception as e:
        return {"error": str(e)}

def set_motor(url, motor_num, angle):
    try:
        r = requests.get(f"{url}/cmd?motor={motor_num}&value={angle}", timeout=2)
        return r.text
    except Exception as e:
        return str(e)

def main():
    print("=" * 50)
    print("           CAT BOT PYTHON CONTROLLER              ")
    print("=" * 50)
    print("Connecting to robot...")
    url = get_base_url()
    print(f"Target URL: {url}")
    print()
    print("Available Commands:")
    print("  Poses:   stand, rest, wave, dance, cute, freaky, bow, pushup, swim, dead, crab")
    print("  Walk:    forward, backward, left, right, stop")
    print("  Faces:   face happy, face angry, face surprised, face excited, face sleepy")
    print("  Motor:   motor <1-8> <angle 0-180>  (e.g., motor 1 90)")
    print("  all <angle> (e.g. all 90)")
    print("  quit to exit")
    print("-" * 50)

    while True:
        try:
            cmd = input("cat-bot> ").strip()
            if not cmd:
                continue
            if cmd.lower() in ["quit", "exit", "q"]:
                break
            
            parts = cmd.split()
            if parts[0] == "face" and len(parts) > 1:
                res = send_command(url, face=parts[1])
                print(f"Face response: {res}")
            elif parts[0] == "motor" and len(parts) == 3:
                res = set_motor(url, int(parts[1]), int(parts[2]))
                print(f"Motor response: {res}")
            elif parts[0] == "all" and len(parts) == 2:
                angle = int(parts[1])
                for m in range(1, 9):
                    set_motor(url, m, angle)
                print(f"All motors set to {angle}°")
            else:
                res = send_command(url, command=cmd)
                print(f"Command response: {res}")
        except KeyboardInterrupt:
            break
        except Exception as ex:
            print(f"Error: {ex}")

if __name__ == "__main__":
    main()
