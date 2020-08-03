#define max_stations  25

/*STATIONS STRUTURA*/
struct stationsSTRUCT  {
  int   ampli;  
  char *info;
  char *stream;
};

struct stationsSTRUCT stacje [max_stations]={
  {85, "NowySwiat", "http://stream.open.fm/368"},
  {85, "TOK-FM",    "https://pl-play.adtonos.com/tok-fm"},
  {85, "RMF_Class", "http://195.150.20.6/rmf_classic"},
  {99, "SwissJaz",  "http://stream.srg-ssr.ch/m/rsj/mp3_128"},
  {85, "Dwojka",    "http://stream3.polskieradio.pl:8902/"},
  {85, "Trojka",    "http://stream3.polskieradio.pl:8904/"},
  {85, "Szczecin",  "http://stream4.nadaje.com:11986/prs.aac"},
  {85, "Zlote",     "http://mainstream.radioagora.pl/tuba9-1.mp3"},
  {99, "ClassFM",   "http://media-the.musicradio.com/ClassicFM"},
  {99, "SwissCls",  "http://stream.srg-ssr.ch/m/rsc_de/mp3_128"},
  {85, "Niezapom",  "http://195.150.20.242:8000/rmf_niezapomniane_melodie"},
  {85, "HaloRadio", "http://s3.radio.co/s6b9e36d1c/listen"},
  {85, "JazzFM",   "http://radiojazzfm.radiokitstream.org/radiojazzfm.mp3"},
  {85, "Sax-ever",  "http://sc-sax4ever.1.fm:10148/;"},
  {85, "101_Jazz",  "http://101smoothjazz.cdnstream1.com/b22139_128mp3"},
  {85, "Del_Mar",   "http://stream.cdm-smoothsax.com:8024/stream-mp3-Smooth"},
  {85, "JazzGroov", "http://east-aac-64.streamthejazzgroove.com/stream"},
  {85, "Jazz663",  "http://east-aac-64.streamthejazzgroove.com/stream"},
  {85, "Tampa-Bay", "http://ams-2.1radio.mk/1radio_smoothjazz_128"},
  {85, "Jazz-Buzz", "http://smoothjazzbuzz.stream.laut.fm/smoothjazzbuzz"},
  {85, "NewOnce",   "http://streamer.radio.co/s93b51ccc1/listen"},
  {85, "Spacja",  "http://pl-play.adtonos.com/8104/radiospacjapl"},
  {85, "RockNews",  "http://radio.rockserwis.fm/live"},
  {85, "Wnet",      "https://media.wnet.fm/wnet.fm"},
  {85, "Paranorm", "http://paranormalium.waw.pl/radio/8040/radio.mp3"}
};
