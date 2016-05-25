// TyAnthoney Morrell CMPS 356
/* This file will perform the animation of the runner dying and will display an
   animation of the death.

   The saveData funtion will save the player name and score to the database 
   once the player dies.

   displayName will return an object to display text to the main function
   where it will be displayed in a moving box that will loop around forever.  
   */

#include "tyM.h"

void saveData(char* u_Name, int score)
{
    struct sockaddr_in *remote;
    int sock;
    int tmpres;
    char *ip;
    char *get;
    char buf[BUFSIZ+1];
    char *host;
    char params[400];
    char tmp[50];
    char page[400];
    char *path;

    host = (char*)HOST;
    path = (char*)PAGE;

    strcpy(params, "?user=");
    strcat(params, u_Name);
    strcat(params, "&score=");
    sprintf(tmp,"%d", score);
    strcat(params, tmp); 
    memcpy(page, path, strlen(path));
    strcat(page, params);
    cout << "The path is " << page << endl; 

    sock = create_tcp_socket();
    ip = get_ip(host);
    fprintf(stderr, "IP is %s\n", ip);
    remote = (struct sockaddr_in *)malloc(sizeof(struct sockaddr_in *));
    remote->sin_family = AF_INET;
    tmpres = inet_pton(AF_INET, ip, (void *)(&(remote->sin_addr.s_addr)));
    if (tmpres < 0)  {
	perror("Can't set remote->sin_addr.s_addr");
	exit(1);
    } else if (tmpres == 0) {
	fprintf(stderr, "%s is not a valid IP address\n", ip);
	exit(1);
    }
    remote->sin_port = htons(PORT);

    if (connect(sock, (struct sockaddr *)remote, sizeof(struct sockaddr)) <0) {
	perror("Could not connect");
	exit(1);
    }
    get = build_get_query(host, page);
    fprintf(stderr, "Query is:\n<<START>>\n%s<<END>>\n", get);

    //Send the query to the server
    int sent = 0;
    // cast as int to remove warning: 
    while (sent < (int)strlen(get)) {
	tmpres = send(sock, get+sent, strlen(get)-sent, 0);
	if (tmpres == -1) {
	    perror("Can't send query");
	    exit(1);
	}
	sent += tmpres;
    }
    //now it is time to receive the page
    memset(buf, 0, sizeof(buf));
    int htmlstart = 0;
    char * htmlcontent;
    while ((tmpres = recv(sock, buf, BUFSIZ, 0)) > 0) {
	if (htmlstart == 0) {
	    /* Under certain conditions this will not work.
	     * If the \r\n\r\n part is splitted into two messages
	     * it will fail to detect the beginning of HTML content
	     */
	    htmlcontent = strstr(buf, "\r\n\r\n");
	    if (htmlcontent != NULL) {
		htmlstart = 1;
		htmlcontent += 4;
	    }
	} else {
	    htmlcontent = buf;
	}
	if (htmlstart) {
	    cout << htmlcontent;;
	}

	memset(buf, 0, tmpres);
    }
    if (tmpres < 0) {
	perror("Error receiving data");
    }
    free(get);
    free(remote);
    free(ip);
    close(sock);
}

void usage()
{
    fprintf(stderr, "USAGE: htmlget host [page]\n\
	    \thost: the website hostname. ex: coding.debuntu.org\n\
	    \tpage: the page to retrieve. ex: index.html, default: /\n");
}


int create_tcp_socket()
{
    int sock;
    if ((sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP)) < 0) {
	perror("Can't create TCP socket");
	exit(1);
    }
    return sock;
}


char *get_ip(char *host)
{
    struct hostent *hent;
    int iplen = 15; //XXX.XXX.XXX.XXX
    char *ip = (char *)malloc(iplen+1);
    memset(ip, 0, iplen+1);
    if ((hent = gethostbyname(host)) == NULL) {
	herror("Can't get IP");
	exit(1);
    }
    if (inet_ntop(AF_INET, (void *)hent->h_addr_list[0], ip,iplen+1) == NULL) {
	perror("Can't resolve host");
	exit(1);
    }
    return ip;
}

char *build_get_query(char *host, char *page)
{
    char *query;
    char *getpage = page;
    // cast to char* to remove warning: deprecated conversion
    char *tpl = (char*)"GET /%s HTTP/1.0\r\nHost:%s\r\nUser-Agent: %s\r\n\r\n";
    if (getpage[0] == '/') {
	getpage = getpage + 1;
	fprintf(stderr,"Removing leading\"/\",converting %s to %s\n",
		page,getpage);
    }
    // -5 is to consider the %s %s %s in tpl and the ending \0
    query = (char *)malloc(strlen(host)+strlen(getpage)+
	    strlen(USERAGENT)+strlen(tpl)-5);

    sprintf(query, tpl, getpage, host, USERAGENT);
    return query;
}
void runnerDeath (Bigfoot &bigfoot, double spritesheetx) 
{
    float wid = 60.0f;
    glBegin(GL_QUADS);
    if (bigfoot.vel[0] > 0.0) {
	glTexCoord2f(0.0f+spritesheetx, 1.0f); glVertex2i(-wid,-wid);
	glTexCoord2f(0.0f+spritesheetx, 0.0f); glVertex2i(-wid, wid);
	glTexCoord2f(0.1f+spritesheetx, 0.0f); glVertex2i( wid,wid);
	glTexCoord2f(0.1f+spritesheetx, 1.0f); glVertex2i( wid,-wid);
    } else {
	glTexCoord2f(0.0f, 1.0f); glVertex2i(-wid,-wid);
	glTexCoord2f(0.0f, 0.0f); glVertex2i(-wid, wid);
	glTexCoord2f(0.1f, 1.0f); glVertex2i( wid, wid);
	glTexCoord2f(0.1f, 0.0f); glVertex2i( wid,-wid);
    }
}

Rect displayName(int location) 
{
    Rect nameBox;
    nameBox.bot = 380;
    nameBox.left = location;
    nameBox.center = 0; 
    return nameBox; 
}

int randomObstacle()
{
    int obstacle = rand() % 3 + 1;
    return obstacle; 
}

float obstacleEffect(int movement, float x, float y, float z, GLuint Texture,
	int &dead, int &image_counter, int &obstacle, int sprite_x,
	int sprite_y, int &booster)
{
    glEnd();
    glPopMatrix();
    //if(counter == somevalue)
    switch(obstacle) {
	case 1:
	    if (image_counter < 200) {
		movement = 8;
		image_counter++;
	    }
	    else { 
		cout << "x is " << x << "\n";
		x -= movement;
		projectImage(x, y, z, Texture);
		if (x < -100 || checkcollison(sprite_x, x, sprite_y, y)) {
		    if(checkcollison(sprite_x, x, sprite_y, y))
			booster = checkcollison(sprite_x, x, sprite_y, y);
		    image_counter = 0;
		    x = 900;
		    movement = 0;
		    obstacle = -1;
		}
	    }
	    break;
	case 2:
	    if(image_counter < 200) {
		movement = 18;
		image_counter++;
	    }
	    else {
		cout << "x is " << x << "\n";
		x -= movement;
		projectImage(x, y, z, Texture);
		if (x < -100 || checkcollison(sprite_x, x, sprite_y, y)) {
		    if(checkcollison(sprite_x, x, sprite_y, y))
			dead = 1; 
		    image_counter = 0;
		    x = 900;
		    movement = 0;
		    obstacle = -1;
		}
	    }
    }
    return x;
}
