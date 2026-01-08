from flask import Flask
from app.database import init_db

app = Flask(__name__)

# Load configurations
app.config.from_object('app.config')

# Initialize database
init_db()

# Import routes
from app import routes