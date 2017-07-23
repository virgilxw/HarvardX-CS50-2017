import cs50

def main():

    #prompt user for an integer between 1 and 
    while True:
        print("Height: ", end = "")
        n = cs50.get_int()
        
        if n > 1 or n < 23:
            break
        
    for i in range(n):
        for j in range(n-i-1):
            print(" ", end ="")
        for k in range(i+1):
            print("#", end="")
            
        print("  ", end="")
        
        for l in range(i+1):
            print("#", end="")
        print("")
        
if __name__ == "__main__":
    main()