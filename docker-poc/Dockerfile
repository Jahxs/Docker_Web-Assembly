# Step 1: Base image from official Alpine Linux with Nginx
FROM nginx:alpine

# Step 2: Copy the custom static HTML website into the Nginx public web directory
COPY index.html /usr/share/nginx/html/index.html

# Step 3: Document that the container listens on port 80
EXPOSE 80

# Nginx starts in foreground by default using the parent image entrypoint
