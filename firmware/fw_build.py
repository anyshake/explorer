from datetime import datetime, timezone


def main():
    current_time = datetime.now(timezone.utc).strftime("%y%m%d%H%M%S")
    print(f"'-DFW_BUILD=\"{current_time}\"'")


if __name__ == "__main__":
    main()
