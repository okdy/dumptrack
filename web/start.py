from flask import Flask, render_template
from database import session, File, Dump

app = Flask(__name__)

@app.route('/')
def hello():
	file = session.query(File).one()

	dumps = session.query(Dump).filter_by(file_id=file.id).all()

	return render_template('index.html', file=file, dumps=dumps)