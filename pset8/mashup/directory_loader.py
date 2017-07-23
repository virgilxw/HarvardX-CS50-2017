import os
import sys
import helpers

from cs50 import SQL
import sqlite3

def main():
    # load raw data into memory
    directory = os.path.join(sys.path[0], "US.txt")
    file = open(directory, "r")
    
    #set db address
    db = SQL("sqlite:///mashup.db")
    
    # iterate through raw data
    for line in file:
    
        # buffer
        line_buffer = []
        
        #split line_buffer by tabs
        line_buffer = line.split("\t", 12)
        
        # console output
        print("Writing into places the following data point| country_code:", line_buffer[0], "|postal_code:", line_buffer[1], "|place_name:", line_buffer[2], "|admin_name1:", line_buffer[3], "|admin_code1:", line_buffer[4], "|admin_name2:", line_buffer[5], "|admin_code2:", line_buffer[6], "|admin_name3:", line_buffer[7], "|admin_code3:", line_buffer[8], "|latitude:", line_buffer[9], "|longitude:", line_buffer[10], "|accuracy:", line_buffer[11])
        
        # write line_buffer into db
        db.execute("INSERT INTO places (country_code, postal_code, place_name, admin_name1, admin_code1, admin_name2, admin_code2, admin_name3, admin_code3, latitude, longitude, accuracy) VALUES (:country_code, :postal_code, :place_name, :admin_name1, :admin_code1, :admin_name2, :admin_code2, :admin_name3, :admin_code3, :latitude, :longitude, :accuracy);", country_code = line_buffer[0], postal_code = line_buffer[1], place_name = line_buffer[2], admin_name1 = line_buffer[3], admin_code1 = line_buffer[4], admin_name2 = line_buffer[5], admin_code2 = line_buffer[6], admin_name3 = line_buffer[7], admin_code3 = line_buffer[8], latitude = line_buffer[9], longitude = line_buffer[10], accuracy = line_buffer[11])

    #close file
    file.close()

if __name__ == "__main__":
    main()