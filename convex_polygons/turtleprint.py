import turtle
import csv

# Define the file path to read from
file_path = 'polygon.csv'

scale_factor= 20
# Step 1: Read the coordinate pairs from the CSV file
coordinate_pairs = []
with open(file_path, 'r') as file:
    csv_reader = csv.reader(file)
    '''next(csv_reader)'''  # Skip the header if present
    for row in csv_reader:
        x1, y1, x2, y2 = float(row[0]), float(row[1]), float(row[2]), float(row[3])
        coordinate_pairs.append(((x1, y1), (x2, y2)))

# Step 2: Set up the Turtle environment
screen = turtle.Screen()
t = turtle.Turtle()

# Set the turtle's speed
t.speed(5)
t.pencolor("blue")

# Step 3: Move the turtle to connect each pair of points
for (x1, y1), (x2, y2) in coordinate_pairs:
    t.penup()
    t.goto(4 * x1, 4 * y1)  # Move to the first point
    t.pendown()
   # t.write(f"({x1}, {y1})", font=("Arial", 12, "normal"))  # Display the first coordinate
    t.goto(4 * x2, 4 * y2)  # Draw a line to the second point
    #t.write(f"({x2}, {y2})", font=("Arial", 12, "normal"))  # Display the second coordinate

# Keep the window open until it is closed by the user
#turtle.done()

file_path = 'line_segments.csv'

scale_factor= 20
# Step 1: Read the coordinate pairs from the CSV file
coordinate_pairs = []
with open(file_path, 'r') as file:
    csv_reader = csv.reader(file)
    '''next(csv_reader)'''  # Skip the header if present
    for row in csv_reader:
        x1, y1, x2, y2 = float(row[0]), float(row[1]), float(row[2]), float(row[3])
        coordinate_pairs.append(((x1, y1), (x2, y2)))

# Step 2: Set up the Turtle environment
screen = turtle.Screen()
t = turtle.Turtle()

# Set the turtle's speed
t.speed(5)
t.pencolor("red")

# Step 3: Move the turtle to connect each pair of points
for (x1, y1), (x2, y2) in coordinate_pairs:
    t.penup()
    t.goto(4 * x1, 4 * y1)  # Move to the first point
    t.pendown()
   # t.write(f"({x1}, {y1})", font=("Arial", 12, "normal"))  # Display the first coordinate
    t.goto(4 * x2, 4 * y2)  # Draw a line to the second point
    #t.write(f"({x2}, {y2})", font=("Arial", 12, "normal"))  # Display the second coordinate

# Keep the window open until it is closed by the user
turtle.done()
