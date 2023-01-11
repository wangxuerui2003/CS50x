import os

from cs50 import SQL
from flask import Flask, flash, redirect, render_template, request, session, url_for
from flask_session import Session
from tempfile import mkdtemp
from werkzeug.security import check_password_hash, generate_password_hash

from datetime import datetime

from helpers import apology, login_required, lookup, usd

# Configure application
app = Flask(__name__)

# Ensure templates are auto-reloaded
app.config["TEMPLATES_AUTO_RELOAD"] = True

# Custom filter
app.jinja_env.filters["usd"] = usd

# Configure session to use filesystem (instead of signed cookies)
app.config["SESSION_PERMANENT"] = False
app.config["SESSION_TYPE"] = "filesystem"
Session(app)

# Configure CS50 Library to use SQLite database
db = SQL("sqlite:///finance.db")

# Make sure API key is set
if not os.environ.get("API_KEY"):
    raise RuntimeError("API_KEY not set")


@app.after_request
def after_request(response):
    """Ensure responses aren't cached"""
    response.headers["Cache-Control"] = "no-cache, no-store, must-revalidate"
    response.headers["Expires"] = 0
    response.headers["Pragma"] = "no-cache"
    return response


@app.route("/")
@login_required
def index():
    """Show portfolio of stocks"""

    sum = 0

    rows = db.execute("SELECT * FROM portfolio WHERE user_id = :user_id", user_id=session['user_id'])

    # Append the latest stock price and total holding into the rows of dictionaries of stock
    for row in rows:
        info = lookup(row['symbol'])
        price = info['price']
        total = price * int(row['shares'])
        row['price'] = usd(price)
        row['total'] = usd(total)
        sum += total

    # Query cash from the user table
    cash = float(db.execute("SELECT cash FROM users WHERE id = :id", id=session['user_id'])[0]['cash'])

    sum += cash

    return render_template("index.html", rows=rows, cash=usd(cash), sum=usd(sum))


@app.route("/buy", methods=["GET", "POST"])
@login_required
def buy():
    """Buy shares of stock"""

    # Check user input
    if request.method == "POST":
        symbol = request.form.get("symbol")
        if not symbol:
            return apology("Please input a symbol!", 400)

        try:
            shares = int(request.form.get("shares"))
        except ValueError:
            return apology("Please input an integer for shares!", 400)

        if not shares or shares <= 0:
            return apology("Please input a positive integer for share!", 400)

        # Loop up from the API
        info = lookup(symbol)

        if not info:
            return apology("Quote does not exist!", 400)

        price = info['price']

        # Check if there is enough cash in user's account
        cash = float(db.execute("SELECT cash FROM users WHERE id = :id", id=session["user_id"])[0]['cash'])

        if cash < price * shares:
            return apology("Not enough cash in your account!", 400)

        cash -= price * shares

        # Insert the transaction into the transactions table
        db.execute("INSERT INTO transactions (user_id, symbol, shares, price, time) VALUES (:user_id, :symbol, :shares, :price, :time)",
                   user_id=session["user_id"], symbol=symbol, shares=shares, price=price, time=datetime.now().strftime('%Y-%m-%d %H:%M:%S'))

        # UPDATE the cash for the user
        db.execute("UPDATE users SET cash = :cash WHERE id=:user_id", cash=str(cash), user_id=session['user_id'])

        # UPDATE the portfolio of the user
        # Get the old shares number
        old = db.execute("SELECT shares FROM portfolio WHERE user_id = :user_id AND symbol = :symbol",
                         user_id=session['user_id'], symbol=symbol)

        if old:
            # Update shares if there is already a column for the symbol
            old_shares = int(old[0]['shares'])
            db.execute("UPDATE portfolio SET shares = :new_shares WHERE user_id = :user_id AND symbol = :symbol",
                       new_shares=str(old_shares+shares), user_id=session['user_id'], symbol=symbol)
        else:
            # Insert new stock of the symbol into portfolio
            db.execute("INSERT INTO portfolio (user_id, symbol, name, shares) VALUES (:user_id, :symbol, :name, :shares)",
                       user_id=session['user_id'], symbol=symbol, name=info['name'], shares=str(shares))

        flash(f"Bought {shares} shares of {symbol} stock!")
        return redirect("/")

    else:
        return render_template("buy.html")


@app.route("/history")
@login_required
def history():
    """Show history of transactions"""

    rows = db.execute("SELECT * FROM transactions WHERE user_id = :user_id ORDER BY time DESC", user_id=session['user_id'])

    # Beautify the prices
    for row in rows:
        row['price'] = usd(float(row['price']))

    return render_template("history.html", rows=rows)


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
        rows = db.execute("SELECT * FROM users WHERE username = ?", request.form.get("username"))

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

    if request.method == "POST":
        symbol = request.form.get("symbol")

        if not symbol:
            return apology("Please input a symbol!", 400)

        quote = lookup(symbol)

        if not quote:
            return apology("Quote does not exist!", 400)

        quote['price'] = usd(quote['price'])
        return render_template("quote.html", quote=quote)

    else:
        return render_template("quote.html")


@app.route("/register", methods=["GET", "POST"])
def register():
    """Register user"""

    # Forget any user_id
    session.clear()

    # User reached route via POST (as by submitting a form via POST)
    if request.method == "POST":

        username = request.form.get("username")
        password = request.form.get("password")
        confirm_password = request.form.get("confirmation")

        # Ensure username was submitted
        if not username:
            return apology("must provide username", 400)

        # Ensure password was submitted
        elif not password:
            return apology("must provide password", 400)

        elif confirm_password != password:
            return apology("password and confirm password must be the same", 400)

        # Query database for username
        rows = db.execute("SELECT * FROM users WHERE username = ?", request.form.get("username"))

        # Ensure username does not exist
        if len(rows) != 0:
            return apology("username already taken", 400)

        # Insert the username and hash password into the database
        db.execute("INSERT INTO users (username, hash) VALUES (?, ?)", username, generate_password_hash(password))

        # Redirect user to home page
        flash("Registered!")
        return redirect("/")

    # User reached route via GET (as by clicking a link or via redirect)
    else:
        return render_template("register.html")


@app.route("/sell", methods=["GET", "POST"])
@login_required
def sell():
    """Sell shares of stock"""

    # Check user input
    if request.method == "POST":
        symbol = request.form.get("symbol")
        if not symbol:
            return apology("Please input a symbol!", 400)

        try:
            shares = int(request.form.get("shares"))
        except ValueError:
            return apology("Please input a number for shares!", 400)

        if not shares or shares <= 0:
            return apology("Please input a positive integer for shares!", 400)

        # Loop up from the API
        info = lookup(symbol)

        if not info:
            return apology("Quote does not exist!", 400)

        # Check if the user hold the stock
        old = db.execute("SELECT shares FROM portfolio WHERE user_id = :user_id AND symbol = :symbol",
                         user_id=session['user_id'], symbol=symbol)

        if not old:
            return apology("Trying to hack my system huh?", 403)

        # Check if the user is holding enough shares
        old_shares = int(old[0]['shares'])
        if old_shares < shares:
            return apology("Not enough shares held!", 400)

        # get price & update cash
        price = info['price']
        cash = float(db.execute("SELECT cash FROM users WHERE id = :id", id=session["user_id"])[0]['cash'])
        cash += price * shares

        # Insert the transaction into the transactions table
        db.execute("INSERT INTO transactions (user_id, symbol, shares, price, time) VALUES (:user_id, :symbol, :shares, :price, :time)",
                   user_id=session["user_id"], symbol=symbol, shares=-shares, price=price, time=datetime.now().strftime('%Y-%m-%d %H:%M:%S'))

        # UPDATE the cash for the user
        db.execute("UPDATE users SET cash = :cash WHERE id=:user_id", cash=str(cash), user_id=session['user_id'])

        # UPDATE portfolio, delete the column if the user sold all of the shares of a certain symbol
        if old_shares == shares:
            db.execute("DELETE FROM portfolio WHERE user_id = :user_id AND symbol = :symbol",
                       user_id=session['user_id'], symbol=symbol)
        else:
            db.execute("UPDATE portfolio SET shares = :new_shares WHERE user_id = :user_id AND symbol = :symbol",
                       new_shares=str(old_shares-shares), user_id=session['user_id'], symbol=symbol)

        flash(f"Sold {shares} of {symbol} stock!")
        return redirect("/")

    else:
        symbols = []

        rows = db.execute("SELECT symbol FROM portfolio WHERE user_id = :user_id", user_id=session['user_id'])

        print(rows)

        for row in rows:
            symbols.append(row['symbol'])

        return render_template("sell.html", symbols=symbols)


@app.route("/recharge", methods=["GET", "POST"])
@login_required
def recharge():
    """ Recharge cash for the user """

    if request.method == "POST":

        # Verify password
        password = request.form.get("password")
        if not password:
            return apology("Try to hack my system huh?", 403)
        pwhash = db.execute("SELECT hash FROM users WHERE id = :id", id=session['user_id'])[0]['hash']
        if not check_password_hash(pwhash, password):
            flash("Incorrect password!")
            return redirect("/recharge")

        # Get the amount of recharge
        amount = request.form.get("amount")
        try:
            amount = float(amount)
        except ValueError:
            return apology("Trying to hack my system huh?", 403)

        # Calculate the new cash
        old_cash = float(db.execute("SELECT cash FROM users WHERE id = :id", id=session['user_id'])[0]['cash'])
        new_cash = old_cash + amount

        # UPDATE the cash in the users table
        db.execute("UPDATE users SET cash = :cash WHERE id = :id", cash=str(new_cash), id=session["user_id"])

        # Insert the transaction into the transactions table
        db.execute("INSERT INTO transactions (user_id, symbol, shares, price, time) VALUES (:user_id, :symbol, :shares, :price, :time)",
                   user_id=session["user_id"], symbol="RECHARGE", shares=0, price=amount, time=datetime.now().strftime('%Y-%m-%d %H:%M:%S'))

        flash(f"${amount} Recharged successfully!")
        return redirect("/")

    else:
        return render_template("recharge.html")
