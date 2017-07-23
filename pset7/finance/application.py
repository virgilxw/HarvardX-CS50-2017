from cs50 import SQL
from flask import Flask, flash, redirect, render_template, request, session, url_for
from flask_session import Session
from passlib.apps import custom_app_context as pwd_context
from tempfile import mkdtemp

from helpers import *

# configure application
app = Flask(__name__)

# ensure responses aren't cached
if app.config["DEBUG"]:
    @app.after_request
    def after_request(response):
        response.headers["Cache-Control"] = "no-cache, no-store, must-revalidate"
        response.headers["Expires"] = 0
        response.headers["Pragma"] = "no-cache"
        return response

# custom filter
app.jinja_env.filters["usd"] = usd

# configure session to use filesystem (instead of signed cookies)
app.config["SESSION_FILE_DIR"] = mkdtemp()
app.config["SESSION_PERMANENT"] = False
app.config["SESSION_TYPE"] = "filesystem"
Session(app)

# configure CS50 Library to use SQLite database
db = SQL("sqlite:///finance.db")

@app.route("/")
@login_required
def index():
    #cache username
    user_id = session.get("user_id")
    
    #render breakdown
    #create list of list of symbols
    shares_dict = db.execute("SELECT symbol, SUM(quantity), price FROM transactions WHERE user_id=:user_id GROUP BY symbol;", user_id=user_id)
    
    #breakdown is a libary, containing libaries, to be passed onto view.
    breakdown=[]
    assets = 0
    
    #iterate through list of symbols, generating each element of breakdown to be passed on to view
    for element in shares_dict:
        
        #lookup quote for current element
        quote = lookup(element["symbol"])
        
        #lookup last transaction
        last_trans = db.execute("SELECT quantity, price, MAX(transac_id) FROM transactions WHERE user_id=:user_id AND symbol = :symbol;", user_id=user_id, symbol=element["symbol"])

        #add to total assets
        assets += quote["price"] * element['SUM(quantity)']
        
        #writing to buffer
        row_buffer = {}
        row_buffer['symbol'] = element["symbol"]
        row_buffer['Name'] = quote["name"]
        row_buffer['quantity'] = element['SUM(quantity)']
        row_buffer['current_quote'] = quote["price"]
        row_buffer['Stock_Value'] = usd(quote["price"] * element['SUM(quantity)'])
        try:
            row_buffer['last_transac_qty'] = last_trans[0]['quantity']
            row_buffer['last_transac_price'] = last_trans[0]['price']
        except:
            row_buffer['last_transac_qty']= "never"
            row_buffer['last_transac_price']= "never"
        
        #append to breakdown
        breakdown.append(row_buffer)
    
    # render overview
    username = db.execute("SELECT username FROM users WHERE id=:user_id;", user_id=user_id)
    wallet = db.execute("SELECT wallet FROM users WHERE id=:user_id;", user_id=user_id)
    overview = {
        "username": username[0]["username"],
        "assets": usd(assets),
        "wallet": usd(wallet[0]["wallet"])
    }
    
    return render_template("index.html", overview=overview, breakdown=breakdown)

@app.route("/buy", methods=["GET", "POST"])
@login_required
def buy():
    # if user reached route via POST (as by submitting a form via POST)
    if request.method == "POST":
        if not request.form.get("symbol"):
            return apology("must provide symbol")

        # ensure password was submitted
        elif not request.form.get("quantity"):
            return apology("must provide quantity")
        
        # ensure quantity is float
        try:
            quantity = float(request.form.get("quantity"))
            
        except:
            return apology("learn how to type numbers")

        # seek quote from yahoo
        quote = lookup(request.form.get("symbol"))
        
        # returns apology is stock not found
        if not quote:
            return apology("quote not found on Yahoo")
        
        #subtract wallet
        cost = quantity * quote['price']
        wallet = db.execute("SELECT wallet FROM users WHERE id=:user_id;", user_id=session.get("user_id"))
        wallet = float(wallet[0]['wallet'])
        if wallet > cost:
            db.execute("UPDATE users SET wallet=:wallet WHERE id = :user_id;", wallet=wallet-cost, user_id=session.get("user_id"))
        else:
            return apology("You are too poor")
            
        # format price as USD
        quote['price'] = usd(quote['price'])
        
        # load transaction into table
        db.execute("INSERT INTO transactions (USER_ID, SYMBOL, QUANTITY, PRICE) VALUES (:user_id, :symbol, :quantity, :price)", user_id = session.get("user_id"), symbol = quote['symbol'], quantity = quantity, price = quote['price'])
    
        # generate message
        message = ''.join(("Bought ", str(quantity), " shares of ", quote['name'], "(", quote['symbol'], ")", " for ", str(cost)))
        
        return render_template("buy.html", message=message)

    # else if user reached route via GET (as by clicking a link or via redirect)
    else:
        return render_template("buy.html")

@app.route("/history")
@login_required
def history():
    
    #render breakdown
    #create list of list of symbols
    transac_hist = db.execute("SELECT symbol, quantity, price FROM TRANSACTIONS WHERE user_id = :user_id ORDER BY transac_id DESC", user_id=session.get("user_id"))
    
    #breakdown is a libary, containing libaries, to be passed onto view.
    hist_output = []
    assets = 0
    
    #iterate through list of symbols, generating each element of breakdown to be passed on to view
    for element in transac_hist:
        
        #lookup quote for current element
        quote = lookup(element["symbol"])
        
        #writing to buffer
        row_buffer = {}
        row_buffer['symbol'] = element["symbol"]
        row_buffer['name'] = quote["name"]
        row_buffer['quantity'] = element['quantity']
        row_buffer['price'] = element['price']
        
        #append to breakdown
        hist_output.append(row_buffer)
    
    # render overview
    username = db.execute("SELECT username FROM users WHERE id=:user_id;", user_id=session.get("user_id"))
    wallet = db.execute("SELECT wallet FROM users WHERE id=:user_id;", user_id=session.get("user_id"))
    overview = {
        "username": username[0]["username"],
        "wallet": usd(wallet[0]["wallet"])
    }
    
    return render_template("history.html", overview=overview, hist_output=hist_output)

@app.route("/login", methods=["GET", "POST"])
def login():
    """Log user in."""

    # forget any user_id
    session.clear()

    # if user reached route via POST (as by submitting a form via POST)
    if request.method == "POST":

        # ensure username was submitted
        if not request.form.get("username"):
            return apology("must provide username")

        # ensure password was submitted
        elif not request.form.get("password"):
            return apology("must provide password")

        # query database for username
        rows = db.execute("SELECT * FROM users WHERE username = :username", username=request.form.get("username"))

        # ensure username exists and password is correct
        if len(rows) != 1 or not pwd_context.verify(request.form.get("password"), rows[0]["hash"]):
            return apology("invalid username and/or password")

        # remember which user has logged in
        session["user_id"] = rows[0]["id"]

        # redirect user to home page
        return redirect(url_for("index"))

    # else if user reached route via GET (as by clicking a link or via redirect)
    else:
        return render_template("login.html")

@app.route("/logout")
def logout():
    """Log user out."""

    # forget any user_id
    session.clear()

    # redirect user to login form
    return redirect(url_for("login"))

@app.route("/quote", methods=["GET", "POST"])
def quote():
    # if user reached route via POST (as by submitting a form via POST)
    if request.method == "POST":
        
        # ensure quote was submitted
        if not request.form.get("symbol"):
            return apology("must provide symbol for quote")
        
        # seek quote from yahoo    
        quote = lookup(request.form.get("symbol"))
        
        # returns apology is stock not found
        if not quote:
            return apology("quote not found on Yahoo")
        
        # format price as USD
        quote['price'] = usd(quote['price'])
        
        # return results
        return render_template("quote.html", quote=quote)

    # else if user reached route via GET (as by clicking a link or via redirect)
    else:
        return render_template("quote.html")

@app.route("/register", methods=["GET", "POST"])
def register():
    """Register user."""
    # manipulate the information the user has submitted
    if request.method == "POST":
        
        # ensure username was submitted
        if not request.form.get("username"):
            return apology("must provide username")
            
        # ensure password was submitted
        if not request.form.get("password"):
            return apology("must provide password")
        
        # ensure password confirmation was submitted
        if not request.form.get("confirm_pw"):
            return apology("must provide password confirmation")
            
        # ensure password and confirmation match
        if request.form.get("confirm_pw") != request.form.get("password"):
            return apology("passwords must match")
            
        # store the hash of the password and not the actual password that was typed in
        password = request.form.get("password")
        hash = pwd_context.hash(password)
        
        # username must be a unique field
        result = db.execute("INSERT INTO users (username, hash) VALUES (:username, :hash)", \
        username = request.form.get("username"), hash=hash)
        if not result:
            return apology("username already in use")
        
        # store their id in session to log them in automatically
        user_id = db.execute("SELECT id FROM users WHERE username IS :username",\
        username = request.form.get("username"))
        return redirect(url_for("index"))
    
        # clear cache
        session.clear()
    
    else:
        return render_template("register.html")

@app.route("/sell", methods=["GET", "POST"])
def sell():
    """Register user."""
    # manipulate the information the user has submitted
    if request.method == "POST":
        
        if not request.form.get("symbol"):
            return apology("must provide symbol")

        # ensure password was submitted
        elif not request.form.get("quantity"):
            return apology("must provide quantity")
        
        # ensure quantity is float
        try:
            sold_qty = float(request.form.get("quantity"))
            
        except:
            return apology("learn how to type numbers")

        # seek quote from yahoo
        quote = lookup(request.form.get("symbol"))
        
        # returns apology is stock not found
        if not quote:
            return apology("quote not found on Yahoo")
        
        #generate owned quantity
        owned_qty = db.execute("SELECT SUM(quantity) FROM transactions WHERE user_id=:user_id AND symbol=:symbol;", user_id=session.get("user_id"), symbol = request.form.get("symbol"))
        owned_qty = float(owned_qty[0]['SUM(quantity)'])
        
        # update wallet
        cost = sold_qty * quote["price"]
        if sold_qty < owned_qty:
            db.execute("UPDATE users SET wallet = wallet + :cost WHERE id=:user_id;", user_id = session.get("user_id"), cost = cost)
        else:
            return apology("you don't own that quantity of stock")
            
        # format price as USD
        quote['price'] = usd(quote['price'])
        
        # load transaction into transactions
        db.execute("INSERT INTO transactions (USER_ID, SYMBOL, QUANTITY, PRICE) VALUES (:user_id, :symbol, :quantity, :price);", user_id = session.get("user_id"), symbol = quote['symbol'], quantity = 0 - sold_qty, price = quote['price'])
    
        # generate message
        message = ''.join(("Sold ", str(sold_qty), " shares of ", quote['name'], "(", quote['symbol'], ")", " for ", str(cost)))

    else:
        message = ''
    
    #cache username
    user_id = session.get("user_id")
    
    #render breakdown
    #create list of list of symbols
    shares_dict = db.execute("SELECT symbol, SUM(quantity), price FROM transactions WHERE user_id=:user_id GROUP BY symbol;", user_id=user_id)
    
    #breakdown is a libary, containing libaries, to be passed onto view.
    breakdown=[]
    assets = 0
    
    #iterate through list of symbols, generating each element of breakdown to be passed on to view
    for element in shares_dict:
        
        #lookup quote for current element
        quote = lookup(element["symbol"])
        
        #lookup last transaction
        last_trans = db.execute("SELECT quantity, price, MAX(transac_id) FROM transactions WHERE user_id=:user_id AND symbol = :symbol;", user_id=user_id, symbol=element["symbol"])

        #add to total assets
        assets += quote["price"] * element['SUM(quantity)']
        
        #writing to buffer
        row_buffer = {}
        row_buffer['symbol'] = element["symbol"]
        row_buffer['Name'] = quote["name"]
        row_buffer['quantity'] = element['SUM(quantity)']
        row_buffer['current_quote'] = quote["price"]
        row_buffer['Stock_Value'] = usd(quote["price"] * element['SUM(quantity)'])
        try:
            row_buffer['last_transac_qty'] = last_trans[0]['quantity']
            row_buffer['last_transac_price'] = last_trans[0]['price']
        except:
            row_buffer['last_transac_qty']= "never"
            row_buffer['last_transac_price']= "never"
        
        #append to breakdown
        breakdown.append(row_buffer)
    
    # render overview
    username = db.execute("SELECT username FROM users WHERE id=:user_id;", user_id=user_id)
    wallet = db.execute("SELECT wallet FROM users WHERE id=:user_id;", user_id=user_id)
    overview = {
        "username": username[0]["username"],
        "assets": usd(assets),
        "wallet": usd(wallet[0]["wallet"])
    }
    
    #render nil message
    return render_template("sell.html", overview=overview, breakdown=breakdown, message=message)