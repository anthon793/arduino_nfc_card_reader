from flask import request, jsonify
from app import app
from app.database import get_db_connection

@app.route('/nfc', methods=['POST'])
def receive_nfc():
    data = request.json
    card_uid = data.get("nfc_uid")

    if not card_uid:
        return jsonify({"error": "Invalid data"}), 400

    conn = get_db_connection()
    cur = conn.cursor()
    cur.execute("INSERT INTO nfc_logs (card_uid) VALUES (%s)", (card_uid,))
    conn.commit()
    cur.close()
    conn.close()

    return jsonify({"message": "Data saved", "card_uid": card_uid}), 201
