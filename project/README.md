# TODO++

#### Video Demo:  https://youtu.be/S7UniQaEouc

#### Description:

**Todo-list and Habit managing app**

------

Technologies used:
* HTML
* CSS
* Bootstrap
* Javascript
* Sqlite
* Flask


##### Menus
Main, Habits, Settings

##### Functionalities
1. User can register and have to login to use the app
2. In the index page, user have his tasks in a grid
3. Show the expired tasks and have a button to move to today
4. User can check the tasks or give up the tasks
5. A menu for habits, user can set a habit and have time span
6. Users can change their password and colorscheme

### Installation

```
pip3 install flask
pip3 install flask_session
pip3 install cs50
```
For Linux:
```
sudo apt install sqlite3
```
For Windows:
```
Install WSL2 Bruh
```

### Setup

First you have to set the FLASK_APP environment variable.

Standard Way
```
export FLASK_APP=app.py
```

For Windows
```
set FLASK_APP=app.py
```

For Powershell
```
$env:FLASK_APP = "app.py"
```


### Running it

To execute the program run the following
```
flask run
```

OR

```
python3 app.py
```
to start the backend part of the application.

#### Images
Tasks page
![example_img](assets/example.png)

Habits Page
![habit_img](assets/habit.png)

Firstly you have to login in order to access anything else of the app.

After you login, you can check your tasks on the index (main) page, you can add tasks by clicking the big add button at the bottom right corner.

The tasks you've created today will be marked as expired if you don't finish it today, but you can still refresh it to be a task for today!

You can manage your habit in the habit menu, unfortunately you can have only one ongoing habit, so you gotta focus!!

You may change colorscheme and password on the settings dropdown menu. You may also logout there.

#### Thank you
Thank you Mr. David Malan for teaching this amazing course! Thank you Brian for walking me through all the psets details! And of course thank you Doug Loyd for helping me understand more useful stuff besides the lectures!

### About CS50
CS50 is a openware course from Havard University and taught by David J. Malan

Introduction to the intellectual enterprises of computer science and the art of programming. This course teaches students how to think algorithmically and solve problems efficiently. Topics include abstraction, algorithms, data structures, encapsulation, resource management, security, and software engineering. Languages include C, Python, and SQL plus students’ choice of: HTML, CSS, and JavaScript (for web development).

Thank you for all CS50.

- Where I get CS50 course?
https://cs50.harvard.edu/x/2020/

[LinkedIn Felipe Nieto](https://www.linkedin.com/in/felipe-antonio-nieto-curcio-9b865116a/)

## Hope you enjoy! This is CS50!
