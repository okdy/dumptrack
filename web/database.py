from sqlalchemy import create_engine, Column, Integer, String
from sqlalchemy.ext.declarative import declarative_base
from sqlalchemy.orm import sessionmaker

from settings import DATABASE as db

# Database connect uri
URI = "{}://{}:{}@{}/{}".format(db['ENGINE'], db['USER'], db['PASS'], db['HOST'], db['DB'])

# sqlalchemy application
engine = create_engine(URI)
Base = declarative_base()

Session = sessionmaker(bind=engine)
session = Session()


'''
File table

save history of what file

id
primary key

file_name
file name of disassembly

description
default description
'''
class File(Base):
	__tablename__ = 'file'

	id = Column(Integer, primary_key=True)
	file_name = Column(String)
	description = Column(String)



'''
Dump table

Store reigster dump data

id
primary key

file_id
foreign key to File.id but not normlization (only logical)

address
opcode address (save hex to decimal)

opcode
hex to disassebly

eax
...
edi
register value
'''
class Dump(Base):
	__tablename__ = 'dump'

	id = Column(Integer, primary_key=True)
	file_id = Column(Integer)
	address = Column(Integer)
	opcode = Column(String)
	eax = Column(Integer)
	ebx = Column(Integer)
	ecx = Column(Integer)
	edx = Column(Integer)
	ebp = Column(Integer)
	esp = Column(Integer)
	esi = Column(Integer)
	edi = Column(Integer)