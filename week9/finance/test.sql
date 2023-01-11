CREATE TABLE portfolio (
    user_id INTEGER,
    symbol VARCHAR(255) NOT NULL,
    name VARCHAR(255) NOT NULL,
    shares INTEGER NOT NULL,
    FOREIGN KEY(user_id) REFERENCES users(id)
);

CREATE TABLE transactions (
    user_id INTEGER,
    symbol VARCHAR(255) NOT NULL,
    shares INTEGER NOT NULL,
    price REAL NOT NULL,
    time DATETIME,
    FOREIGN KEY(user_id) REFERENCES users(id)
);