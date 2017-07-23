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
    
    #generate tuple AZ09
    AZ09 = string.digits + string.ascii_lowercase + string.ascii_uppercase
    
    #generate password to test
    for i in AZ09:
        
        for j in AZ09:
            
            for k in AZ09:
            
                for l in AZ09:
                    
                    #generate password
                    password = i + j + k + l
                    
                    #test password
                    print("testing", format(password))
                    if crypt.crypt(password, salt) == hash1[1]:
                        if format(password):
                            print (format(hash1[0]), format(password))
                            return True
        
    return False
    
if __name__ == "__main__":
    main()