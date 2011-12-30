import smtplib,datetime
from email.mime.text import MIMEText
from email.mime.multipart import MIMEMultipart
mailto_list=["usher2007@foxmail.com"]
mail_host="smtp.qq.com"
mail_user="*********"
mail_pass="*********"
mail_postfix="qq.com"

def send_mail(to_list,sub,content):
	
	me = mail_user+"<"+mail_user+"@"+mail_postfix+">"
	msg = MIMEMultipart('alternative')
	message = MIMEText(content, 'HTML')
	msg['Subject'] = sub
	msg['From'] = me
	msg['To'] = ";".join(to_list)
	msg.attach(message)
	try:
		s = smtplib.SMTP()
		s.connect(mail_host)
		s.ehlo_or_helo_if_needed()
		s.login(mail_user,mail_pass)
		s.sendmail(me, to_list, msg.as_string())
		s.close()
		return True
	except Exception, e:
		print str(e)
		return False

if __name__ == '__main__':
	today = datetime.datetime.today()
	content = "\
			<html>\
				<body>\
					<p style=\"font-weight:bold;font-size:24px;\">Hey, yubo</p>\
					<p>Today is " + today.strftime("%B %d, %A") + ". <a style=\"font-weight:bold;font-size:24px;\">Don't</a> forget the Weekly Report!</p>\
				</body>\
			</html>\
			"

	send_mail(mailto_list, "Weekly Report Reminder", content)


