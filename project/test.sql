-- CREATE TABLE users (
-- 	id INTEGER PRIMARY KEY,
-- 	username TEXT NOT NULL,
-- 	hash TEXT NOT NULL
-- )

CREATE TABLE tasks (
    user_id INTEGER,
	content TEXT,
    time DATE,
    done INTEGER NOT NULL DEFAULT 0,
    FOREIGN KEY(user_id) REFERENCES users(id)
);

-- CREATE TABLE habits (
--     user_id INTEGER,
-- 	content TEXT,
--     start_date DATE,
--     span INTEGER NOT NULL DEFAULT 30,
--     today_done INTEGER NOT NULL DEFAULT 0,
--     done INTEGER NOT NULL DEFAULT 0,
--     FOREIGN KEY(user_id) REFERENCES users(id)
-- );