import smtplib,datetime
from email.mime.text import MIMEText
mailto_list=["usher2007@foxmail.com"]
mail_host="smtp.qq.com"
mail_user="523977091"
mail_pass="zhzl2135360"
mail_postfix="qq.com"

def send_mail(to_list,sub,content):
	
	me = mail_user+"<"+mail_user+"@"+mail_postfix+">"
	msg = MIMEText(content)
	msg['Subject'] = sub
	msg['From'] = me
	msg['To'] = ";".join(to_list)
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
	content = "Today is " + today.strftime("%B %d, %A") + ". Don't forget sending weekly report!"
	send_mail(mailto_list, "Weekly Report Reminder", content)


