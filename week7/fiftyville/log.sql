-- Keep a log of any SQL queries you execute as you solve the mystery.

-- Select the description of the crime scene from the crime_scene_reports table
SELECT	description
FROM	crime_scene_reports
WHERE
	year = 2021 AND month = 7 AND day = 28 AND street = "Humphrey Street";

-- Select the witnesses' transcripts from the interview table
SELECT	transcript
FROM	interviews
WHERE
	year = 2021 AND month = 7 AND day = 28 AND transcript LIKE "%bakery%";

-- Select the name of the thief by the time and location of atm_transaction
SELECT	people.name
FROM	bank_accounts
JOIN	atm_transactions AS trans
ON		trans.account_number = bank_accounts.account_number
JOIN	people
ON		bank_accounts.person_id = people.id
WHERE	trans.year = 2021 AND trans.month = 7 AND trans.day = 28
AND	trans.atm_location = "Leggett Street"
AND	trans.transaction_type = "withdraw"
AND	people.id IN (
	-- Select the id of people who has flight on the day after theft and also the earliest flight
	SELECT	people.id
	FROM	passengers
	JOIN	flights
	ON		passengers.flight_id = flights.id
	JOIN	people
	ON		people.passport_number = passengers.passport_number
	JOIN	airports
	ON		airports.id = flights.origin_airport_id
	WHERE	airports.city LIKE "fiftyville"
	AND	flights.year = 2021 AND flights.month = 7 AND flights.day = 29
	AND	people.passport_number IN (
		-- Select the passport_number of the people who exit the car park within 10mins after the theft
		-- And called someone by the way
		SELECT	DISTINCT(people.passport_number)
		FROM	phone_calls
		JOIN	people
		ON		people.phone_number = phone_calls.caller
		WHERE	year = 2021 AND month = 7 AND day = 28 AND duration < 60
		AND	caller IN (
			SELECT	people.phone_number
			FROM	bakery_security_logs
			JOIN	people
			ON		people.license_plate = bakery_security_logs.license_plate
			WHERE
				bakery_security_logs.activity LIKE "exit"
				AND year = 2021 AND month = 7 AND day = 28 AND hour = 10 AND minute >= 15 AND minute <= 25
		)
	)
	AND flights.hour = 8 AND flights.minute = 20
)
-- Thief: Bruce

-- Select the destination of the flight on the day after the theft
SELECT	airports.city
FROM	airports
WHERE	airports.id IN (
	SELECT	flights.destination_airport_id
	FROM	flights
	JOIN	passengers
	ON		flight_id = flights.id
	WHERE	passengers.passport_number IN (
		SELECT	passport_number
		FROM	people
		WHERE	people.name = "Bruce"
	)
)
-- Destination: New York City

-- Select the people Bruce called right after the theft (his accomplice)
SELECT people.name
FROM	people
JOIN	phone_calls
ON	phone_calls.receiver = people.phone_number
WHERE	phone_calls.year = 2021 AND phone_calls.month = 7 AND phone_calls.day = 28 AND phone_calls.duration < 60
AND	phone_calls.caller IN (
	SELECT	phone_number
	FROM	people
	WHERE	name = "Bruce"
)
-- Accomplice: Robin
