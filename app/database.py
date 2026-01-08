import psycopg2
from app.config import DATABASE

def get_db_connection():
    conn = psycopg2.connect(**DATABASE)
    return conn

def init_db():
    conn = get_db_connection()
    cur = conn.cursor()
    cur.execute("""
        CREATE TABLE IF NOT EXISTS nfc_logs (
            id SERIAL PRIMARY KEY,
            card_uid TEXT NOT NULL,
            timestamp TIMESTAMP DEFAULT CURRENT_TIMESTAMP
        )
    """)
    conn.commit()
    cur.close()
    conn.close()
