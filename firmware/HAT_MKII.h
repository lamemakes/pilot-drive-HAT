#define DEFAULT_TIMEOUT 30
#define STATUS_LED 1
#define FET_CTL 4
#define ACC_OPTO 3
#define SCL 2
#define SDA 0
#define SLAVE_ADDR 0x20

int initDDRB = (1 << FET_CTL) | (0 << ACC_OPTO) | (0 << SCL) | (1 << STATUS_LED) | (0 << SDA);


int byte; // Used to read the byte after the address byte
int read_data; // Used when data is address to slave

int timeout = DEFAULT_TIMEOUT; // Value recieved from master, defaults to DEFAULT_TIMEOUT in case no I2C value is recieved.

int data_recieved; // Flashes the LED if data was recieved