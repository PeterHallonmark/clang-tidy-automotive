import sys

def main():
    violations = False  # exempel

    if violations:
        print("Copyright violation found!")
        sys.exit(1)  
    else:
        print("All good!")
        sys.exit(0)

if __name__ == "__main__":
    main()
