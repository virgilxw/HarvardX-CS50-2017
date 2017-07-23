import cs50

def main():
    #user input
    print("Number: ", end="")
    card_no = cs50.get_int()
    counter = 0
    checksum = 0
    prefix = 0
    prefix_switch = False
    card_supplier = "INVALID"
    
    #iterate through digits by modulo
    while (card_no > 0):
        
        #strip last digit
        counter += 1
        last_digit = card_no % 10
        
        #checksum
        if counter % 2 == 0:
            last_digit *= 2
            if last_digit >= 10:
                checksum += 1 + (last_digit - 10)
            else:
                checksum += last_digit
                
        else:
            checksum += last_digit
            
        #return card number, minus last digit
        card_no = card_no // 10
        
        #supplier check
        if card_no < 99 and prefix_switch == False:
            prefix_switch = True
            
            if (card_no // 10 == 4) and (counter == 14 or 11):
                card_supplier = "VISA"
                
            elif (card_no == 51 or 52 or 53 or 54 or 55) and counter == 14:
                card_supplier = "MASTERCARD"
    
            elif (card_no == 34 or 37) and counter == 13:
                card_supplier = "AMEX"
                
    #reject card_no with wrong checksum
    if checksum % 10 != 0:
        card_supplier = "INVALID"
        
    #output
    print(card_supplier)

if __name__ == "__main__":
    main()