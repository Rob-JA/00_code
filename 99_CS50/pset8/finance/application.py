import os
import datetime
import re

from cs50 import SQL
from flask import Flask, flash, jsonify, redirect, render_template, request, session
from flask_session import Session
from tempfile import mkdtemp
from werkzeug.exceptions import default_exceptions, HTTPException, InternalServerError
from werkzeug.security import check_password_hash, generate_password_hash

from helpers import apology, login_required, lookup, usd

# Configure application
app = Flask(__name__)

# Ensure templates are auto-reloaded
app.config["TEMPLATES_AUTO_RELOAD"] = True

# Ensure responses aren't cached
@app.after_request
def after_request(response):
    response.headers["Cache-Control"] = "no-cache, no-store, must-revalidate"
    response.headers["Expires"] = 0
    response.headers["Pragma"] = "no-cache"
    return response


# Custom filter
app.jinja_env.filters["usd"] = usd

# Configure session to use filesystem (instead of signed cookies)
app.config["SESSION_FILE_DIR"] = mkdtemp()
app.config["SESSION_PERMANENT"] = False
app.config["SESSION_TYPE"] = "filesystem"
Session(app)

# Configure CS50 Library to use SQLite database
db = SQL("sqlite:///finance.db")

# Make sure API key is set
if not os.environ.get("API_KEY"):
    raise RuntimeError("API_KEY not set")


@app.route("/")
@login_required
def index():
    """Show portfolio of stocks"""

    # get users cash position.
    cash = db.execute("SELECT cash FROM users WHERE id = :id", id=session['user_id'])
    cash = cash[0]['cash']

    # gets users portfolio info.
    index = db.execute("SELECT * FROM portfolio WHERE id = :id", id=session['user_id'])

    total_value = 0
    portfolio_value = 0

    # for each position held, get company name, price, total value and insert it into index.
    for row in range(len(index)):
        quote = lookup(index[row]['symbol'])
        index[row]['company'] = quote['name']
        index[row]['price'] = usd(quote['price'])
        value = round((index[row]['shares'] * quote['price']), 2)
        total_value = round(total_value + value, 2)
        index[row]['value'] = usd(value)

    # get cash position, total stock value and total portfolio value.
    portfolio_value = usd(cash + total_value)
    cash = usd(cash)
    total_value = usd(total_value)

    # return index page, parsing required variables.
    return render_template("index.html", index=index, cash=cash, total_value=total_value, portfolio_value=portfolio_value)


@app.route("/buy", methods=["GET", "POST"])
@login_required
def buy():
    """Buy shares of stock"""
    # if quote page is selected, go to quote html page.
    if request.method == "GET":
        return render_template("buy.html")

    # if the submit button is pressed on the quote page, do the following.
    else:
        # gets the ticker symbol from the user.
        symbol = request.form.get("symbol")

        # validates usage of symbol.
        if not symbol or re.search('[0-9]', symbol) is True:
            return apology("you must enter a symbol", 403)

        # gets the number of shares from the user.
        shares = float(request.form.get("shares"))

        if (shares) % 1 != 0:
            return apology("you must enter a positive integer not a float", 403)

        # change number into an integer.
        shares = int(shares)

        # validates number of shares as a positive integer.
        if not shares or shares < 1:
            return apology("you must enter a positive integer", 403)

        # runs lookup function on user's ticker symbol.
        quote = lookup(symbol)

        # validates whether the ticker symbol could be found.
        if quote == None:
            return apology("stock not found", 403)

        # get the amount of cash user has remaining from db.
        cash = db.execute("SELECT cash FROM users WHERE id = :id", id=session['user_id'])

        # check user can afford the purchase.
        check_cash = float(cash[0]['cash'])

        # sum the total price of the transaction
        total = float(quote['price'] * shares)

        # set the purchase/sale type to purchase.
        purchasesale = "purchase"

        # get the current date and time.
        now = datetime.datetime.now()

        # check if transaction total is greater than user's funds. If so return an apology.
        if total > check_cash:
            return apology("you do not have the required funds to purchase this stock", 403)

        # otherwise insert info into transaction table in db.
        else:
            db.execute("INSERT INTO transactions (id, symbol, price, shares, purchasesale, timestamp) VALUES (:id, :symbol, :price, :shares, :purchasesale, :timestamp)",
                       id=session['user_id'], symbol=symbol, price=quote['price'], shares=shares, purchasesale=purchasesale, timestamp=now)

            # update remaining cash in users table.
            db.execute("UPDATE users SET cash = cash - :total WHERE id = :id",
                       total=total, id=session['user_id'])

            # check if user already has shares of the purchased stock.
            rows = db.execute("SELECT * FROM portfolio WHERE id = :id AND symbol = :symbol",
                              id=session['user_id'], symbol=symbol)

            # if they don't own any of the stock, insert new shares into db;
            if len(rows) == 0:
                db.execute("INSERT INTO portfolio (id, symbol, shares) VALUES (:id, :symbol, :shares)",
                           id=session['user_id'], symbol=symbol, shares=shares)

            # if user already has stock then update portfolio table.
            else:
                db.execute("UPDATE portfolio SET shares = shares + :shares WHERE id = :id AND symbol = :symbol",
                           shares=shares, id=session['user_id'], symbol=symbol)

        # redirect user to home page.
        return redirect("/")


@app.route("/history")
@login_required
def history():
    """Show history of transactions"""

    # gets users portfolio info.
    index = db.execute("SELECT * FROM transactions WHERE id = :id", id=session['user_id'])

    total_value = 0
    portfolio_value = 0

    # for each position held, get company name, price, total value and insert it into index.
    for row in range(len(index)):
        quote = lookup(index[row]['symbol'])
        index[row]['company'] = quote['name']
        index[row]['price'] = usd(quote['price'])
        value = round((index[row]['shares'] * quote['price']), 2)
        index[row]['value'] = usd(value)

    # redirect user to history page.
    return render_template("history.html", index=index)


@app.route("/login", methods=["GET", "POST"])
def login():
    """Log user in"""

    # Forget any user_id
    session.clear()

    # User reached route via POST (as by submitting a form via POST)
    if request.method == "POST":

        # Ensure username was submitted
        if not request.form.get("username"):
            return apology("must provide username", 403)

        # Ensure password was submitted
        elif not request.form.get("password"):
            return apology("must provide password", 403)

        # Query database for username
        rows = db.execute("SELECT * FROM users WHERE username = :username",
                          username=request.form.get("username"))

        # Ensure username exists and password is correct
        if len(rows) != 1 or not check_password_hash(rows[0]["hash"], request.form.get("password")):
            return apology("invalid username and/or password", 403)

        # Remember which user has logged in
        session["user_id"] = rows[0]["id"]

        # Redirect user to home page
        return redirect("/")

    # User reached route via GET (as by clicking a link or via redirect)
    else:
        return render_template("login.html")


@app.route("/logout")
def logout():
    """Log user out"""

    # Forget any user_id
    session.clear()

    # Redirect user to login form
    return redirect("/")


@app.route("/quote", methods=["GET", "POST"])
@login_required
def quote():
    """Get stock quote."""
    # if quote page is selected, go to quote html page.
    if request.method == "GET":
        return render_template("quote.html")

    # if the submit button is pressed on the quote page, do the following.
    else:
        # gets the ticker symbol from the user.
        symbol = request.form.get("symbol")

        if bool(re.search('[0-9]', symbol)) is True:
            return apology("you must enter a symbol not a number", 403)

        # validates usage of symbol.
        if not symbol:
            return apology("you must enter a symbol", 403)

        # runs lookup function on user's ticker symbol.
        quote = lookup(symbol)

        # validates whether the ticker symbol could be found.
        if quote == None:
            return apology("stock not found", 403)

        # if the ticker symbol is found, return the quoted page which contains price.
        else:
            return render_template("quoted.html", quote=quote)


@app.route("/register", methods=["GET", "POST"])
def register():
    """Register user"""
    # if register button is pressed, go to register page.
    if request.method == "GET":
        return render_template("register.html")

    # if the submit button is pressed on the register page, do the following.
    else:
        # validate the usage of a username.
        username = request.form.get("username")
        if not username:
            return apology("you must enter a username", 403)

        # validate the usage of a password.
        password = request.form.get('password')
        if not password:
            return apology("you must enter a password", 403)

        # validate password condition of containing a number.
        if re.search('[0-9]', password) is None:
            return apology("your password must have a number in it", 403)

        # validate password condition of containing a capital letter.
        elif re.search('[A-Z]', password) is None:
            return apology("your password must have a capital letter in it", 403)

        # get confirmation of password and ensure a match.
        confirmation = request.form.get('confirmation')
        if password != confirmation:
            return apology("passwords do not match", 403)

        # hash users password.
        hash = generate_password_hash(password)

        # check for unique username
        usernamecheck = db.execute("SELECT * FROM users WHERE username = :username", username=username)

        if len(usernamecheck) != 0:
            return apology("username already taken.", 403)

        # insert user's details into db.
        new_user = db.execute("INSERT INTO users (username, hash) VALUES (:username, :hash)", username=username, hash=hash)

        # redirect user to index page.
        return redirect("/")


@app.route("/sell", methods=["GET", "POST"])
@login_required
def sell():
    """Sell shares of stock"""

    # if sell page is selected, go to sell html page.
    if request.method == "GET":
        return render_template("sell.html")

    # if the sell button is pressed on the sell page, do the following.
    else:
        # gets the ticker symbol from the user.
        symbol = request.form.get("symbol")

        # validates usage of symbol.
        if not symbol:
            return apology("you must enter a symbol", 403)

        # gets the number of shares from the user.
        shares = request.form.get("shares")

        # validates number of shares as a positive integer.
        if not shares:
            return apology("you must enter a positive integer", 403)

        if (float(shares) % 1) != 0:
            return apology("you must enter a positive integer not a float", 403)

        shares = int(shares)

        if shares < 1:
            return apology("you must enter a positive integer", 403)

        # check if user already has shares of the purchased stock.
        rows = db.execute("SELECT * FROM portfolio WHERE id = :id AND symbol = :symbol",
                          id=session['user_id'], symbol=symbol)

        # if they don't own any of the stock, return apology.
        if len(rows) == 0:
            return apology("you do not own any of these shares", 403)

        # gets the number of shares owned by user.
        owned_shares = db.execute("SELECT shares FROM portfolio WHERE id = :id AND symbol = :symbol",
                                  id=session['user_id'], symbol=symbol)

        owned_shares = int(owned_shares[0]['shares'])

        # check if user has the enough shares to sell the desired amount. If so return an apology.
        if shares > owned_shares:
            return apology("you do not have that quantity of shares to sell", 403)

        # update the number of shares owned in the portfolio table in db.
        db.execute("UPDATE portfolio SET shares = shares - :shares WHERE id = :id AND symbol = :symbol",
                   shares=shares, id=session['user_id'], symbol=symbol)

        # get the number of remaining shares owned by user from the table.
        remaining_shares = db.execute("SELECT shares FROM portfolio WHERE id = :id AND symbol = :symbol",
                                      id=session['user_id'], symbol=symbol)

        # if the number of shares owned now is 0, remove row from portfolio.
        if remaining_shares[0]['shares'] == 0:
            db.execute("DELETE FROM portfolio WHERE id = :id AND symbol = :symbol", id=session['user_id'], symbol=symbol)

        # runs lookup function on user's ticker symbol.
        quote = lookup(symbol)

        # calculate the total value of the sale.
        total = float(quote['price'] * shares)

        # update remaining cash in users table.
        db.execute("UPDATE users SET cash = cash + :total WHERE id = :id", total=total, id=session['user_id'])

        # set the purchase/sale type to purchase.
        purchasesale = "sale"

        # get the current date and time.
        now = datetime.datetime.now()

        # insert transaction into transaction table in db.
        db.execute("INSERT INTO transactions (id, symbol, price, shares, purchasesale, timestamp) VALUES (:id, :symbol, :price, :shares, :purchasesale, :timestamp)",
                   id=session['user_id'], symbol=symbol, price=quote['price'], shares=shares, purchasesale=purchasesale, timestamp=now)

        # redirect user to index page.
        return redirect("/")


def errorhandler(e):
    """Handle error"""
    if not isinstance(e, HTTPException):
        e = InternalServerError()
    return apology(e.name, e.code)


# Listen for errors
for code in default_exceptions:
    app.errorhandler(code)(errorhandler)
