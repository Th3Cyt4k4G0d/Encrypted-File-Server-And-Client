from Crypto.Cipher import AES
from Crypto.PublicKey import RSA
import Crypto.Cipher
from Crypto.Cipher import PKCS1_OAEP
import socket
import os
import selectors
import sqlite3
import uuid
import consts

import deserializer
import serializer

FILENAME = 'port.info'
IP = '127.0.0.1'
PORT = 1234
sel = selectors.DefaultSelector()


def accept(sock, mask):
    conn, addr = sock.accept()
    print('accepted ', conn, ' from ', addr)
    conn.setblocking(False)
    sel.register(conn, selectors.EVENT_READ, read)


def read(conn, mask):
    sql = sqlite3.connect('server.db')
    c = sql.cursor()

    # register user
    data = conn.recv(1024)
    request = deserializer.deserialize_request(data)
    code = request[consts.CODE] # get code

    # HANDLE ACCORDING TO CODE:
    if code == consts.REGISTER:
        handle_1100(conn, c, request)
    elif code == consts.PUBLIC_KEY:
        handle_1101(conn, c, request)
    elif code == consts.SEND_FILE:
        pass
    elif code == consts.CRC_VALID:
        conn.close()
    elif code == consts.CRC_INVALID:
        pass
    elif code == consts.CRC_INVALID_2:
        conn.close()

    sql.commit()
    sql.close()


def main():
    server()


def server():
    global PORT

    try:
        PORT = getPort()
    except:
        print("Port file does not exist, using default port 1234.")
    sock = socket.socket()
    sock.bind((IP, PORT))
    sock.listen(100)
    sock.setblocking(False)
    sel.register(sock, selectors.EVENT_READ, accept)

    conn = sqlite3.connect('server.db')
    c = conn.cursor()
    #createTable(c)

    conn.commit()
    conn.close()

    while True:
        events = sel.select()
        for key, mask in events:
            callback = key.data
            callback(key.fileobj, mask)


def createClientsTable(cur):
    cur.execute(''' CREATE TABLE IF NOT EXISTS clients(
    id TEXT PRIMARY KEY, 
    username TEXT NOT NULL, 
    PublicKey BLOB, 
    LastSeen TEXT NOT NULL,
    AES BLOB
    )''')


def createFilesTable(cur):
    cur.execute(''' CREATE TABLE IF NOT EXISTS files(
        id TEXT PRIMARY KEY, 
        username TEXT NOT NULL, 
        path TEXT NOT NULL, 
        VERIFIED BOOLEAN NOT NULL,
        )''')


def userExists(cur, username):
    cur.execute("SELECT EXISTS(SELECT 1 FROM clients WHERE username=?)", (username,))
    return cur.fetchone()[0]


def addUser(cur, UUID, username):
    cur.execute("INSERT INTO clients VALUES(?, ?, ?, ?, ?)", (UUID, username, 'X', False))


def addFiles(cur, UUID, username, path, verified):
    cur.execute("INSERT INTO files VALUES(?, ?, ?, ?)", (UUID, username, path, verified))


def updateUser(cur, UUID, public_key, AESKey):
    cur.execute("UPDATE clients SET PublicKey=?, LastSeen=(SELECT datetime('now', 'localtime')), AES=? WHERE id=?", (public_key, AESKey, UUID))


def handle_1100(conn, c, request):
    name = request[consts.NAME].decode()
    if userExists(c, name):
        # print error message
        pass

    userUUID = createUUID()
    #addUser(c, userUUID, name)
    response = serializer.serialize_2100('3', 2100, consts.ID_SIZE, userUUID)  # reply with UUID
    conn.sendall(response)


def handle_1101(conn, c, request):
    key = RSA.importKey(request[consts.KEY])
    userUUID = request[consts.ID].decode('ascii')
    #updateUser(c, userUUID, str(key), True)
    #AESKey = AES.new(os.urandom(16), AES.MODE_EAX)

    AES_key = os.urandom(consts.AES_KEY_LEN)
    print(AES_key)
    encryptor = PKCS1_OAEP.new(key)
    AES_encrypted = encryptor.encrypt(AES_key)
    print(str(AES_encrypted))
    response = serializer.serialize_2102('3', 2102, len(userUUID) + len(AES_encrypted), userUUID, str(AES_encrypted))
    conn.sendall(response)


def createUUID():
    # uuid1 has a zero chance of collisions since it takes date/time into account and we do not generate
    # a great amount of UUIDs at the same time.
    return str(uuid.uuid1().hex)


def getPort():
    with open(FILENAME) as f:
        return f.read().strip()


if __name__ == '__main__':
    main()
