-- list the names of all people who have directed a movie that received a rating of at least 9.0.
SELECT	DISTINCT(name)
FROM	people
JOIN	directors
ON		people.id = directors.person_id
JOIN	ratings
ON		ratings.movie_id = directors.movie_id
WHERE	ratings.rating >= 9.0