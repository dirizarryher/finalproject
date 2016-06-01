// TyAnthoney Morrell CMPS 356
/* This file will perform 
 * 1. Perform the runner dying animation
 * 2. Save user data to the database
 * 3. Perform all of the obstacle/boost animations
 * 4. Modify scores based on progress
 * 5. Provide the case number for each obstacle/boost to be called 
 */
#include "tyM.h"

int mod = 1; 
int scoreMod = 1;
int showAlien = 0; 
int boostMod = 0; 
int moneyBoost = 0; 
int lifeBoost = 0;
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

    if (connect(sock, (struct sockaddr *)remote, 
                sizeof(struct sockaddr)) <0) {
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
    if ((sock = socket(AF_INET, SOCK_STREAM, 
                    IPPROTO_TCP)) < 0) {
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
    if (inet_ntop(AF_INET, (void *)hent->h_addr_list[0], 
                ip,iplen+1) == NULL) {
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
        glTexCoord2f(0.0f+spritesheetx, 1.0f); 
        glVertex2i(-wid,-wid);
        glTexCoord2f(0.0f+spritesheetx, 0.0f); 
        glVertex2i(-wid, wid);
        glTexCoord2f(0.1f+spritesheetx, 0.0f); 
        glVertex2i( wid,wid);
        glTexCoord2f(0.1f+spritesheetx, 1.0f); 
        glVertex2i( wid,-wid);
    } else {
        glTexCoord2f(0.0f, 1.0f); 
        glVertex2i(-wid,-wid);
        glTexCoord2f(0.0f, 0.0f); 
        glVertex2i(-wid, wid);
        glTexCoord2f(0.1f, 1.0f); 
        glVertex2i( wid, wid);
        glTexCoord2f(0.1f, 0.0f); 
        glVertex2i( wid,-wid);
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

void scoreModifier(int &score)
{
    if (score > 2000) {
        score += 1;
    }
    if (score > 4000) {
        score += 2;
    }
    if (score > 6000) {
        score += 3;
    }
    if (score > 8000) {
        score += 4;
        lifeBoost = 0;
    }
    if (score > 9000) {
        score += 5;
        lifeBoost = 0;
    }
    if (score > 12000) {
        score += 10;
    }
    if (score > 15000) {
        score += 15;
        lifeBoost = 0;
    }
    if (score > 18000) {
        score += 16;
        lifeBoost = 0;
    }
    if (score > 25000) {
        score += 18;
        lifeBoost = 0;
    }
    if (score > 35000) {
        score += 20;
        lifeBoost = 0;
    }
    if (score > 50000) {
        score += 25;
        lifeBoost = 0;
    }
    if (score > 100000) {
        score += 50;
        lifeBoost = 0;
    }
}

int randomObstacle()
{
    int obstacle = rand() % 9 + 1;
    return obstacle; 
}

float obstacleEffect(int &movement, float x, float y, float z, GLuint Texture,
        int &dead, int &image_counter, int &obstacle, int sprite_x, int &score,
        int sprite_y, int &booster, double diff, double &spritex, int slide)
{
    int collision = 0;
    glEnd();
    glPopMatrix();
    float wid = 50.0f*diff;
    //if(counter == somevalue)
    if (score > 1000+scoreMod) {
        if (movement < 20) {
            movement++;
            scoreMod += 200;
            cout << "Speed increased\n";
        }
    }
    switch (obstacle) {
        case 1:
            if (image_counter < 50) {
                image_counter++;
                x = 1200;
            }
            else {
                if (score > 1400+boostMod) {
                    x -= movement;
                    projectImage(x*diff, y*diff, z, Texture, diff);
                    if (x < -100 || checkcollison(sprite_x, x*diff, 
                                sprite_y, y*diff, diff)) {
                        if (checkcollison(sprite_x,x*diff,sprite_y,
                                    y*diff,diff))
                            booster = checkcollison(sprite_x, x*diff, 
                                    sprite_y, y*diff, diff);
                        image_counter = 0;
                        boostMod += boostMod+200;
                        x = 900;
                        obstacle = -1;
                    }
                }
                else 
                    obstacle = 2;
            }
            break;
        case 2:
            if (image_counter < 50) {
                image_counter++;
                x = 1200;
            }
            else {
                x -= movement;
                projectImage(x*diff, y*diff, z, Texture, diff);
                if (x < -100 || checkcollison(sprite_x, x*diff, 
                            sprite_y, y*diff, diff)) {
                    if (checkcollison(sprite_x, x*diff, sprite_y, 
                                y*diff, diff))
                        dead = 1; 
                    else
                        score += 10;
                    image_counter = 0;
                    x = 900;
                    obstacle = -1;
                }
            }
            break;
        case 3:
            if (image_counter < 50) {
                image_counter++;
                x = -100;
            }
            else {
                if (score > 1000+mod ) {
                    x += movement;
                    projectImage(x*diff, (y+300)*diff, z, Texture, 
                            diff);
                    if (x > 1200 ) {
                        image_counter = 0;
                        x = -100;
                        obstacle = 4;
                        showAlien = 1;
                        mod += 5000;
                    }
                }
                else
                    obstacle = rand() % 9 + 5;
            }
            break;
        case 4:
            if (image_counter < 50) {
                image_counter++;
                x = 1400;
            }
            else {
                if (showAlien && score > 1000+mod) {
                    glPushMatrix();
                    glTranslatef(x*diff, y*diff, z);
                    glBindTexture(GL_TEXTURE_2D, Texture);
                    glEnable(GL_ALPHA_TEST);
                    glAlphaFunc(GL_GREATER, 0.0f);
                    glColor4ub(255,255,255,255);
                    glBegin(GL_QUADS);
                    glTexCoord2f(0.0f-spritex, 1.0f); glVertex2i(-wid,-wid);
                    glTexCoord2f(0.0f-spritex, 0.0f); glVertex2i(-wid, wid);
                    glTexCoord2f(0.1428f-spritex,0.0f);glVertex2i(wid,wid);
                    glTexCoord2f(0.1428f-spritex,1.0f);glVertex2i(wid,-wid);
                    glEnd();
                    glPopMatrix();
                    spritex += .1428;
                    cout << "x is " << x << "\n";
                    x -= movement;
                    if (x < -100*diff || checkcollison(sprite_x, 
                                x*diff, sprite_y, 
                                y*diff, diff)) {
                        if (checkcollison(sprite_x, x*diff, sprite_y, y*diff, 
                                    diff))
                            dead = 1; 
                        else
                            score += 500;
                        image_counter = 0;
                        x = -100;
                        obstacle = -1;
                    }
                }
                else 
                    obstacle = rand() % 7 + 5;
            }
            break;
        case 5:
            if (image_counter < 50) {
                image_counter++;
                x = 1200;
            }
            else {
                x -= movement;
                projectImage(x*diff, (y+35)*diff, z, Texture, diff);
                if (slide) {
                    collision = 20;
                    if (diff > 1)
                        collision = 70;
                }
                if (x < -200 || checkcollison(sprite_x, x*diff, 
                            sprite_y-collision, (y+35)*diff, diff)) {
                    if (checkcollison(sprite_x,
                                x*diff,sprite_y-collision*diff, 
                                (y+35), diff))
                        dead = 1; 
                    else
                        score += 20;
                    image_counter = 0;
                    x = 900;
                    obstacle = -1;
                }
            }
            break;
        case 6:
            if (image_counter < 50) {
                image_counter++;
                x = 1200;
            }
            else {
                x -= movement+2;
                projectImage(x*diff, (y+35)*diff, z, Texture, diff);
                if (slide) {
                    collision = 20;
                    if (diff > 1)
                        collision = 70;
                }
                if (x < -200 || checkcollison(sprite_x, x*diff, 
                            sprite_y-collision, (y+35)*diff, diff)) {
                    if (checkcollison(sprite_x, x*diff, sprite_y-collision, 
                                (y+35)*diff, diff))
                        dead = 1; 
                    else
                        score += 50;
                    image_counter = 0;
                    x = 900;
                    obstacle = -1;
                }
            }
            break;
        case 7:
            if (image_counter < 50) {
                image_counter++;
                x = 1200;
            }
            else {
                x -= movement+1;
                projectImage(x*diff, y*diff, z, Texture, diff);
                if (x < -100 || checkcollison(sprite_x, x*diff, 
                            sprite_y, y*diff, diff)) {
                    if (checkcollison(sprite_x, x*diff, 
                                sprite_y, y*diff, diff))
                        dead = 1; 
                    else
                        score += 50;
                    image_counter = 0;
                    x = 900;
                    obstacle = -1;
                }
            }
            break;
        case 8:
            if (image_counter < 50) {
                image_counter++;
                x = 1200;
            }
            else {
                if (score > 400+boostMod) {
                    x -= movement;
		    cout << x << endl;
                    projectImage(x*diff, y*diff, z, Texture, diff);
                    if (x < -100 || checkcollison(sprite_x, x*diff, 
                                sprite_y, y*diff, diff)) {
                        if (checkcollison(sprite_x,x*diff,sprite_y,
                                    y*diff,diff))
                            booster = checkcollison(sprite_x, x*diff, 
                                    sprite_y, y*diff, diff);
				
                        image_counter = 0;
                        if (booster)
                            score += 1500;
                        boostMod += boostMod+500;
                        x = 900;
                        obstacle = -1;
                    }
                }
                else 
                    obstacle = 2;
            }
            break;
        case 9:
            if (image_counter < 50) {
                image_counter++;
                x = 1200;
            }
            else {
                if (lifeBoost == 0) {
                    x -= movement;
                    projectImage(x*diff, y*diff, z, Texture, diff);
                    if (x < -100 || checkcollison(sprite_x, x*diff, 
                                sprite_y, y*diff, diff)) {
                        if (checkcollison(sprite_x,x*diff,sprite_y,
                                    y*diff,diff))
                            lifeBoost = checkcollison(sprite_x, x*diff, 
                                    sprite_y, y*diff, diff);
                        image_counter = 0;
                        if (lifeBoost) {
                            score += 1;
                        }
                        boostMod += boostMod+500;
                        x = 900;
                        obstacle = -1;
                    }
                }
                else
                    obstacle = 2;
            }
            break;
        default: 
            obstacle = 2;
    }
    return x;
}
