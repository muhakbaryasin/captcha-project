from requests import Request, Session
from pyquery import PyQuery as pq
import uuid
from subprocess import call, check_output
import datetime
import os.path
from time import sleep
from random import randint

class Scraper(object):
	def __init__(self, url, method='GET', id=None, headers=None):
		self.setUrl(url)
		self.setMethod(method)
		self.setParams( {} )
		self.session = Session()
		
		if id is not None:
			self.setId(id)
		
		self.headers = {'User-Agent' : 'Mozilla/5.0 (Windows NT 6.3; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/66.0.3359.139 Safari/537.36'}
		
		if headers is not None:
			self.updateHeaders(headers)
	
	def setMethod(self, method):
		if type(method) is not str and (method != 'GET' or method != 'POST' or method != 'GET_JSON' or method != 'POST_JSON'):
			raise Exception("Method must be a string GET/POST/GET_JSON/POST_JSON")

		self.method = method

	def setUrl(self, url):
		if type(url) is not str:
			raise Exception("Url must be a string.")

		self.url = url

	def setId(self, id):
		if type(url) is not str:
			raise Exception("Id must be a string.")

		self.id = id

	def updateHeaders(self, headers):
		if type(headers) is not dict:
			raise Exception("Headers must be a dict.")
		
		self.headers.update(headers)
	
	def setParams(self, params):
		if type(params) is not dict:
			raise Exception("Params must be a dict.")
	
		self.params = params
	
	def updateParams(self, params):
		if type(params) is not dict:
			raise Exception("Params must be a dict.")
	
		self.params.update(params)
	
	def getParams(self):
		return self.params
	
	def getHeaders(self):
		return self.headers
	
	def getMethod(self):
		return self.method
	
	def execute(self):
		kwargs = {}
		args = []
		
		if self.getMethod() == "GET" or self.getMethod() == "POST":
			args.append(self.getMethod())
			kwargs['data'] = self.getParams()
		elif self.getMethod() == "GET_JSON":
			args.append("GET")
			kwargs['json'] = self.getParams()
		elif self.getMethod() == "POST_JSON":
			args.append("POST")
			kwargs['json'] = self.getParams()
			
		args.append(self.url)
		kwargs['headers'] = self.headers
		
		req = Request(*args, **kwargs)
		
		response = self.session.head(self.url)
		content_type = response.headers['content-type']
		
		prepared = self.session.prepare_request(req)
		print('Request url -> {}'.format(req.url))
		print('Request headers : {}'.format(req.headers))
		print('Request body : {}'.format(prepared.body))
		res = self.session.send(prepared)
		
		return (content_type, res)

def subcall(list_):
	print(datetime.datetime.now().strftime("%Y-%m-%d %H:%M:%S") + " " + (" ".join(list_)))
	call(list_)
	
	#try:
	#	output = check_output(list_)
	#except Exception as e:
	#	output = str(e)
	#return output

if __name__ == "__main__":
	for x in range(0, 1000):
		scraper = Scraper('https://agent.lionair.co.id/LionAirAgentsPortal/Default.aspx')
		res = scraper.execute()
		new_uuid = str(uuid.uuid4())
		
		for y in range(0, 20):
			url_captcha = 'https://agent.lionair.co.id/LionAirAgentsPortal/CaptchaGenerator.aspx'
			scraper.setUrl(url_captcha)
			res2 = scraper.execute()
			
			dir_name = 'unlabeled/'
			
			if not os.path.exists(dir_name):
				subcall(['mkdir', dir_name])
			
			sub_dir_num = 1
			
			while True:
				if not os.path.exists(dir_name + str(sub_dir_num) + '/'):
					subcall(['mkdir', dir_name + str(sub_dir_num) + '/'])
						
				path = dir_name + str(sub_dir_num) + '/' + new_uuid
				
				if not os.path.exists(path):
					print("writing " + path)
					with open(path, 'wb') as f:
						for chunk in res2[1].iter_content(128):
							f.write(chunk)
				else:
					print("udah ada path " + path)
					sub_dir_num += 1
					continue
					
				break
				
			sleep(randint(1,3))
