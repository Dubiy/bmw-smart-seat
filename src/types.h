using namespace std;

struct task {
    uint32_t executeAt;
    std::function<void()> f;

    void operator()() { f(); }
};

typedef struct {
  String ssid;
  int ssid_length = 32;
  int ssid_offset = 0;

  String pass;
  int pass_length = 32;
  int pass_offset = ssid_length + ssid_offset;

  String secret;
  int secret_length = 32;
  int secret_offset = pass_length + pass_offset;

  String title;
  int title_length = 32;
  int title_offset = secret_length + secret_offset;

  String crc;
  int crc_length = 3;
  int crc_offset = title_length + title_offset;

  // String brightness; 
  // int brightness_length = 3;
  // int brightness_offset = 131;

  uint32_t apIP = 16820416; // IPAddress apIP(192,168,0,1);//  192 + 168 *2^8 + 0 *2^16 + 1 *2^24
  unsigned int udpPort = 8888;
} Settings;