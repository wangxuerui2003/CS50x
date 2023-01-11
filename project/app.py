from flask import Flask, render_template, request, redirect, flash, session
from flask_session import Session
from utils import login_required, apology
from cs50 import SQL
from werkzeug.security import check_password_hash, generate_password_hash
from datetime import datetime, timedelta

# Flash Message categories
SUCCESS = 'success'
DANGER = 'danger'
INFO = 'info'
WARNING = 'warning'
PRIMARY = 'primary'

app = Flask(__name__)

# Ensure templates are auto-reloaded
app.config["TEMPLATES_AUTO_RELOAD"] = True

# Configure session to use filesystem (instead of signed cookies)
app.config["SESSION_PERMANENT"] = False
app.config["SESSION_TYPE"] = "filesystem"
Session(app)

db = SQL("sqlite:///todo.db")


@app.route("/")
@login_required
def index():
    """ Show the today tasks and expired tasks """

    # Delete the tasks that are done and expired
    db.execute("DELETE FROM tasks WHERE user_id = :user_id AND done = 1 AND DATE(time) = :yesterday",
               user_id=session['user_id'], yesterday=datetime.strftime(datetime.now() - timedelta(1), '%Y-%m-%d'))

    tasks = db.execute("SELECT * FROM tasks WHERE user_id = :id ORDER BY time ASC", id=session['user_id'])
    today = []
    expired = []

    done = 0
    current = 0

    # sort tasks into today and expired
    for task in tasks:
        time = datetime.strptime(task['time'], '%Y-%m-%d')
        if (datetime.today() - time).days == 0:  # check if the task is created today
            today.append(task)
            current += 1
            if task['done'] == 1:
                done += 1
        else:
            expired.append(task)

    return render_template("index.html", current=current, done=done, tasks=today, expired=expired)


@app.route("/create_task", methods=['GET', 'POST'])
@login_required
def create_task():
    """ Create task without refreshing the page using Ajax """
    if request.method == 'POST':
        current_tasks = db.execute("SELECT content FROM tasks WHERE user_id = :user_id", user_id=session['user_id'])
        if len(current_tasks) < 8:
            task = request.form.get("task")
            if task:
                db.execute("INSERT INTO tasks (user_id, content, time) VALUES (:user_id, :task, :time)",
                           user_id=session['user_id'], task=task, time=datetime.now().strftime('%Y-%m-%d'))

        else:
            flash("Too many tasks for today!", WARNING)

    return "Hello!"


@app.route("/delete_task", methods=['GET', 'POST'])
@login_required
def delete_task():
    """ Delete task without refreshing the page using Ajax """
    if request.method == 'POST':
        task = request.form.get("task")
        if task:
            db.execute("DELETE FROM tasks WHERE user_id = :user_id AND content = :task", user_id=session['user_id'], task=task)

    return "Bye!"


@app.route("/check_task", methods=['GET', 'POST'])
@login_required
def check_task():
    """ Check task"""
    if request.method == 'POST':
        task = request.form.get("task")
        if task:
            db.execute("UPDATE tasks SET done = 1 WHERE user_id = :user_id AND content = :task",
                       user_id=session['user_id'], task=task)

    flash("Congratulation!", SUCCESS)
    return redirect("/")


@app.route("/reset_task", methods=['GET', 'POST'])
@login_required
def reset_task():
    """ Reset expired task to today """
    if request.method == 'POST':
        task = request.form.get("task")
        if task:
            db.execute("UPDATE tasks SET time = :time WHERE user_id = :user_id AND content = :task",
                       time=datetime.now().strftime('%Y-%m-%d'), user_id=session['user_id'], task=task)

    flash("Good Attitude! You can do it today!", SUCCESS)
    return redirect("/")


@app.route("/habits", methods=['GET', 'POST'])
@login_required
def habits():
    """ Show habits and add habits """
    if request.method == 'POST':

        current = db.execute("SELECT * FROM habits WHERE user_id = :user_id AND done = 0", user_id=session['user_id'])
        if len(current) >= 1:
            flash("You gotta focus on one habit at a time bro!", WARNING)
            return redirect("/habits")

        habit = request.form.get('habit')
        span = request.form.get('span')

        if not habit:
            flash("Please fill in the habit name!", WARNING)
            return redirect("/habits")

        if not span:
            span = 30

        db.execute("INSERT INTO habits (user_id, content, start_date, span) VALUES (:user_id, :content, :start_date, :span)",
                   user_id=session['user_id'], content=habit, start_date=datetime.now().strftime('%Y-%m-%d'), span=span)

        flash("Habit started successfully! Keep going!", SUCCESS)
        return redirect("/habits")

    else:
        habits = db.execute("SELECT * FROM habits WHERE user_id = :user_id AND done = 0", user_id=session['user_id'])
        if habits:
            for habit in habits:
                if habit['last_done_date']:
                    if (datetime.today() - datetime.strptime(habit['last_done_date'], '%Y-%m-%d')).days == 1:
                        # refresh habit
                        db.execute("UPDATE habits SET today_done = 0 WHERE user_id = :user_id AND content = :content",
                                   user_id=session['user_id'], content=habit['content'])
                    elif (datetime.today() - datetime.strptime(habit['last_done_date'], '%Y-%m-%d')).days > 1:
                        # habit fail
                        db.execute("DELETE FROM habits WHERE user_id = :user_id AND content = :content",
                                   user_id=session['user_id'], content=habit['content'])
                        flash(f"You have failed the habit \"{habit['content']}\", Keep going next time!", DANGER)
                        return redirect("/habits")

        all = db.execute("SELECT * FROM habits WHERE user_id = :user_id", user_id=session['user_id'])

        done = []
        habits = []

        for habit in all:
            if habit['done'] == 0:
                habits.append(habit)
            else:
                done.append(habit)

        return render_template("habits.html", habits=habits, done=done)


@app.route("/check_habit", methods=['GET', 'POST'])
@login_required
def check_habit():
    """ Check task"""
    if request.method == 'POST':
        habit = request.form.get("habit")
        if habit:
            db.execute("UPDATE habits SET today_done = 1 WHERE user_id = :user_id AND content = :habit",
                       user_id=session['user_id'], habit=habit)
            db.execute("UPDATE habits SET last_done_date = :today WHERE user_id = :user_id AND content = :habit",
                       today=datetime.now().strftime('%Y-%m-%d'), user_id=session['user_id'], habit=habit)

            habit_infos = db.execute("SELECT * FROM habits WHERE user_id = :user_id AND content = :habit",
                                     user_id=session['user_id'], habit=habit)
            for habit_info in habit_infos:
                if (datetime.strptime(habit_info['last_done_date'], '%Y-%m-%d') - datetime.strptime(habit_info['start_date'], '%Y-%m-%d')).days == habit_info['span'] - 1:
                    db.execute("UPDATE habits SET done = 1 WHERE user_id = :user_id AND content = :habit",
                               user_id=session['user_id'], habit=habit)
                    flash(f"Congratulations! You have done the habit \"{habit}\"!", PRIMARY)
                    return redirect("/habits")

    flash("Good job! Keep going!", SUCCESS)
    return redirect("/habits")


@app.route("/login", methods=["GET", "POST"])
def login():
    """Log user in"""

    # Forget any user_id
    session.clear()

    # User reached route via POST (as by submitting a form via POST)
    if request.method == "POST":

        # Ensure username was submitted
        if not request.form.get("username"):
            return apology("must provide username", WARNING, "login", 403)

        # Ensure password was submitted
        elif not request.form.get("password"):
            return apology("must provide password", WARNING, "login", 403)

        # Query database for username
        rows = db.execute("SELECT * FROM users WHERE username = ?", request.form.get("username"))

        # Ensure username exists and password is correct
        if len(rows) != 1 or not check_password_hash(rows[0]["hash"], request.form.get("password")):
            return apology("invalid username and/or password", WARNING, "login", 403)

        # Remember which user has logged in
        session["user_id"] = rows[0]["id"]

        # Redirect user to home page
        flash("Login successful!", SUCCESS)
        return redirect("/")

    # User reached route via GET (as by clicking a link or via redirect)
    else:
        return render_template("login.html")


@app.route("/logout")
@login_required
def logout():
    """Log user out"""

    # Forget any user_id
    session.clear()

    # Redirect user to login form
    return redirect("/")


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
            return apology("must provide username", WARNING, "register", 400)

        # Ensure password was submitted
        elif not password:
            return apology("must provide password", WARNING, "register", 400)

        elif confirm_password != password:
            return apology("password and confirm password must be the same", WARNING, "register", 400)

        # Query database for username
        rows = db.execute("SELECT * FROM users WHERE username = ?", request.form.get("username"))

        # Ensure username does not exist
        if len(rows) != 0:
            return apology("username already taken", WARNING, "register", 400)

        # Insert the username and hash password into the database
        db.execute("INSERT INTO users (username, hash) VALUES (?, ?)", username, generate_password_hash(password))

        # Redirect user to home page
        flash("Registered!", SUCCESS)
        return render_template("login.html")

    # User reached route via GET (as by clicking a link or via redirect)
    else:
        return render_template("register.html")


@app.route("/change_password", methods=['GET', 'POST'])
@login_required
def change_password():
    """ Change New Password """

    if request.method == 'POST':
        old = request.form.get('old')
        new = request.form.get('new')

        if not request.form.get("old"):
            return apology("Must provide old password", WARNING, "change_password", 403)

        # Ensure password was submitted
        elif not request.form.get("new"):
            return apology("Must provide new password", WARNING, "change_password", 403)

        # Query database for username
        rows = db.execute("SELECT * FROM users WHERE id = :id", id=session['user_id'])

        # Ensure username exists and password is correct
        if len(rows) != 1 or not check_password_hash(rows[0]["hash"], request.form.get("old")):
            return apology("Old Password incorrect", WARNING, "change_password", 403)

        db.execute("UPDATE users SET hash = :new WHERE id = :id",
                   new=generate_password_hash(request.form.get("new")), id=session['user_id'])

        session.clear()
        flash("Password changed successfully!", SUCCESS)
        return render_template("login.html")

    return render_template("change_password.html")
    

if __name__ == "__main__":
    app.run(debug=True)
