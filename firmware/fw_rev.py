from os import path


def main():
    try:
        file_path = path.join(path.dirname(__file__), "..", ".git", "logs", "HEAD")
        with open(file_path, "r", encoding="utf-8") as file:
            commit = f"git-{file.read().strip().split("\n")[-1].split(" ")[1][:7]}"
            print(f"'-DFW_REV=\"{commit}\"'")
    except:
        fallback = "custombuild"
        print(f"'-DFW_REV=\"{fallback}\"'")


if __name__ == "__main__":
    main()
