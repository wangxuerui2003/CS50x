import os
import requests
import urllib.parse

symbol = input("Symbol: ")

# Contact API
api_key = os.environ.get("API_KEY")
url = f"https://api.iex.cloud/v1/data/core/quote/{urllib.parse.quote_plus(symbol)}?token={api_key}"
response = requests.get(url)
response.raise_for_status()

print(url)

# Parse response
quote = response.json()[0]

# print(quote["companyName"])
# print(float(quote["latestPrice"]))
# print(quote["symbol"])