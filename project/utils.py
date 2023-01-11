from flask import Flask, render_template, session, redirect, flash, url_for
from functools import wraps


def login_required(f):
    """ Decorate routes to require login. """
    @wraps(f)
    def decorated_function(*args, **kwargs):
        if session.get("user_id") is None:
            return redirect("/login")
        return f(*args, **kwargs)
    return decorated_function


def apology(message, category, route, code):
    """ redirect to home page and flash the message """

    flash(message, category)
    return render_template(route + ".html")
