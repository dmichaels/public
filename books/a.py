import requests

# Example: Fetch a list of fake posts
url = "https://jsonplaceholder.typicode.com/posts"
response = requests.get(url)

if response.status_code == 200:
    posts = response.json()  # Convert the response to JSON
    print(posts)
    import pdb ; pdb.set_trace()
    for post in posts[:3]:  # Print only the first 3 posts
        print(f"Title: {post['title']}")
        print(f"Body: {post['body']}\n")
else:
    print("Failed to retrieve data")
