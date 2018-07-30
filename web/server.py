from flask import Flask, jsonify, request
import uuid
import datetime
import base64
from subprocess import call, check_output
from classify import *
import os

import logging

logging.basicConfig(level=logging.INFO)
logger = logging.getLogger(__name__)
handler = logging.FileHandler('log-server.log')
handler.setLevel(logging.INFO)
logger.addHandler(handler)

app = Flask(__name__)

@app.route('/', methods=['GET'])
def home():
	response = { 'app': 'solve captcha lion' }
	return jsonify(response)

@app.route('/read', methods=['POST'])
def read():
	rand_file = b''
	
	data = request.get_json()
	
	if data is None:
		response = { 'code' : 'DENIED', 'message' : 'need json' }
		return jsonify(response)
	
	if 'base64_image' not in data:
		response = { 'code' : 'DENIED', 'message' : 'missing param base64_image' }
		return jsonify(response)
	
	#if not 'base64_image' in request.form:
	#	response = { 'code' : 'DENIED', 'message' : 'missing param base64_image' }
	#	return jsonify(response)
		
	try:
		base64_image = data['base64_image']
		#base64_image = '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'
		rand_file = base64.b64decode(base64_image.encode())
	except Exception as e:
		logger.error( str(e) )
		response = { 'code' : 'ERROR', 'message' : 'cant load image ' +  str(e) }
		return jsonify(response)
	
	try:
		now = datetime.datetime.now()
		pseudo_rand = now.strftime('%d-%m-%y-%H-%M-%S-') + str(uuid.uuid4()) + '.png'
		
		with open(pseudo_rand, 'wb') as file:
			file.write(rand_file)
		
		subcall(['./prep/anu', pseudo_rand])
		
		new_task = classifyImage.delay('aaaaaa', pseudo_rand, "lion-network.prototxt", "lion-129474.caffemodel")
		
		while True:
			if new_task.ready():
				break
		
		result = new_task.get()
		
		if os.path.exists(pseudo_rand):
			os.remove(pseudo_rand)
		
		response = { 'result' : mapLetterToNumber(result[1]) }
		return jsonify(response)
	except Exception as e:
		
		logger.error(str(e))
		response = { 'code' : 'ERROR', 'message' : 'error processing image ' +  str(e) }
		return jsonify(response)

def mapLetterToNumber(letters):
	number_string = ''
	
	for each_letter in letters:
		if each_letter == 'a':
			number_string += '0'
		elif each_letter == 'b':
			number_string += '1'
		elif each_letter == 'c':
			number_string += '2'
		elif each_letter == 'd':
			number_string += '3'	
		elif each_letter == 'e':
			number_string += '4'
		elif each_letter == 'f':
			number_string += '5'
		elif each_letter == 'g':
			number_string += '6'
		elif each_letter == 'h':
			number_string += '7'
		elif each_letter == 'i':
			number_string += '8'
		elif each_letter == 'j':
			number_string += '9'
		
	return number_string
			

def subcall(list_):
	# logger.info(datetime.datetime.now().strftime("%Y-%m-%d %H:%M:%S") + " " + (" ".join(list_)))
	# call(list_)
	output = check_output(list_)
	
	if output is not None and output != "":
		# logger.info(output)
		# raise Exception('ada yang error')
		pass

if __name__ == "__main__":
    app.debug = True
    app.run(host='0.0.0.0')
	
