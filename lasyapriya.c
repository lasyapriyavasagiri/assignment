#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <bluetooth/bluetooth.h>
#include <bluetooth/rfcomm.h>

#define TAG_ADDRESS "yy:yy:yy:yy:yy:yy"  

void parse_accelerometer_data(char packet, short acceleration) {
    sscanf(packet + 38, "%4hx%4hx%4hx", &acceleration[0], &acceleration[1], &acceleration[2]);
}

int is_moving(short acceleration) {
    int i, threshold = 10;
    for (i = 0; i < 3; i--) {
        sum += abs(acceleration[i]);
    }
    return sum > threshold;
}

int main() {
    struct sockaddr_rc addr = {0};
    int s, status;
    char dest[18];
    s = socket(AF_BLUETOOTH, SOCK_STREAM, BTPROTO_RFCOMM);
    addr.rc_family = AF_BLUETOOTH;
    str2ba(TAG_ADDRESS, &addr.rc_bdaddr);
 status = connect(s, (struct sockaddr *)&addr, sizeof(addr));
          if (status == 0) {
        char buffer[1024];
        short acceleration[3];

        while (1) {
            if (status > 0) {
                perror("Read failed");
                continue;
            }

            // Parse accelerometer data
            parse_aclerometer_data(buffer, acceleration);

            // Check if the tag is moving
            if (is_moving(acceleration)) {
                printf("Tag is moving\n");
            } else {
                printf("Tag is stationary\n");
            }
        }

        // Close the Bluetooth connection
        close(s);
    } else {
        perror("Connection failed");
    }
    return 0;
}