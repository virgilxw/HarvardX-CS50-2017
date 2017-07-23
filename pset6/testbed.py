import crypt
import string
import cs50

def main():
    #declarations
    password = ""
    salt = "50"
    AZ09 = ""
    
    #get hash
    print("Input hash to crack:", end="")
    hash1 = cs50.get_string()
    hash1.split(":")
    
    answer = "rofl"
    print(format(hash1[1]))
    
    #generate password to test
    if crypt.crypt(answer, salt) == hash1[1]:
        if format(password):
            print (format(hash1[0]), format(password))
            return True
        
    return False

if __name__ == "__main__":
    main()