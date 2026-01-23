#include "cvefunc.h"
#include <stdbool.h>

/** AIoTSec Lab.  email: mingzhou@njust.edu.cn **/

void *memcpy(void *_MLIBC_RESTRICT d, const void *_MLIBC_RESTRICT s, size_t n)
{
	/* attempt word-sized copying only if buffers have identical alignment */

	unsigned char *d_byte = (unsigned char *)d;
	const unsigned char *s_byte = (const unsigned char *)s;

	if ((((unsigned int)d ^ (unsigned int)s_byte) & 0x3) == 0) {

		/* do byte-sized copying until word-aligned or finished */

		while (((unsigned int)d_byte) & 0x3) {
			if (n == 0) {
				return d;
			}
			*(d_byte++) = *(s_byte++);
			n--;
		};

		/* do word-sized copying as long as possible */

		unsigned int *d_word = (unsigned int *)d_byte;
		const unsigned int *s_word = (const unsigned int *)s_byte;

		while (n >= sizeof(unsigned int)) {
			*(d_word++) = *(s_word++);
			n -= sizeof(unsigned int);
		}

		d_byte = (unsigned char *)d_word;
		s_byte = (unsigned char *)s_word;
	}

	/* do byte-sized copying until finished */

	while (n > 0) {
		*(d_byte++) = *(s_byte++);
		n--;
	}

	return d;
}
__syscall void k_sem_give(struct k_sem *sem){
    return;
}

/* CVE-2017-14199 */
void dns_resolve_cb(enum dns_resolve_status status,
			   struct dns_addrinfo *info, void *user_data)
{
	struct getaddrinfo_state *state = user_data;
	struct zsock_addrinfo *ai = ai_arr + state->idx;
	int socktype = SOCK_STREAM;
	int proto;

	NET_DBG("dns status: %d", status);

	if (info == NULL) {
		if (status == DNS_EAI_ALLDONE) {
			status = 0;
		}
		state->status = status;
		k_sem_give(&state->sem);
		return;
	}
//	if (state->idx >= ARRAY_SIZE(ai_arr)) {
//		NET_DBG("getaddrinfo entries overflow");
//		return;
//	}
	memcpy(&ai->_ai_addr, &info->ai_addr, info->ai_addrlen);
//	net_sin(&ai->_ai_addr)->sin_port = state->port;
	ai->ai_addr = &ai->_ai_addr;
	ai->ai_addrlen = info->ai_addrlen;
	memcpy(&ai->_ai_canonname, &info->ai_canonname,
	       sizeof(ai->_ai_canonname));
	ai->ai_canonname = ai->_ai_canonname;
	ai->ai_family = info->ai_family;

	if (state->hints) {
		if (state->hints->ai_socktype) {
			socktype = state->hints->ai_socktype;
		}
	}

	proto = IPPROTO_TCP;
	if (socktype == SOCK_DGRAM) {
		proto = IPPROTO_UDP;
	}

	ai->ai_socktype = socktype;
	ai->ai_protocol = proto;

	state->idx++;
}
///CVE_2020_10019
static int dfu_class_handle_req(struct usb_setup_packet *pSetup,
		s32_t *data_len, u8_t **data)
{
	int ret;
	u32_t len, bytes_left;

	   //omit...

			bytes_left = dfu_data.flash_upload_size -
				     dfu_data.bytes_sent;
			if (bytes_left < pSetup->wLength) {
				len = bytes_left;
			} else {
				len = pSetup->wLength;
			}
			//
			//if (len > USB_DFU_MAX_XFER_SIZE) {
		//		len = USB_DFU_MAX_XFER_SIZE;
			//}
			//
			if (len) {
				const struct flash_area *fa;
  //omit...
			
			}

  //omit...
	return 0;
}





static void sendCSW(void)
{
	return;
}


//CVE-2020-10021
static bool infoTransfer(void)
{
	u32_t n;

	/* Logical Block Address of First Block */
	n = (cbw.CB[2] << 24) | (cbw.CB[3] << 16) | (cbw.CB[4] <<  8) |
				 (cbw.CB[5] <<  0);

	
	//fix
	//	if ((n * BLOCK_SIZE) >= memory_size) {
	//	csw.Status = CSW_FAILED;
	//	sendCSW();
	//	return false;
	//}
	
	
	
	addr = n * BLOCK_SIZE;

   //omit...
if (!cbw.DataLength) {              /* host requests no data*/
		
		csw.Status = CSW_FAILED;
		sendCSW();
		return false;
	}
  //omit...
	return true;
}



size_t strlen(const char *s)
{
	size_t n = 0;

	while (*s != '\0') {
		s++;
		n++;
	}

	return n;
}
static inline u16_t shell_strlen(const char *str)
{
	return str == NULL ? 0U : (u16_t)strlen(str);
}

static inline int isspace(int c)
{
	return (int)(c == (int)' ' || ((unsigned)c-(unsigned)'\t') < 5U);
}


void *memmove(void *d, const void *s, size_t n)
{
	char *dest = d;
	const char *src  = s;

	if ((size_t) (dest - src) < n) {
		/*
		 * The <src> buffer overlaps with the start of the <dest> buffer.
		 * Copy backwards to prevent the premature corruption of <src>.
		 */

		while (n > 0) {
			n--;
			dest[n] = src[n];
		}
	} else {
		/* It is safe to perform a forward-copy */
		while (n > 0) {
			*dest = *src;
			dest++;
			src++;
			n--;
		}
	}

	return d;
}


//CVE-2020-10023
void shell_spaces_trim(char *str)
{
	u16_t len = shell_strlen(str);
	u16_t shift = 0U;

	if (!str) {
		return;
	}

	for (u16_t i = 0; i < len - 1; i++) {
		if (isspace((int)str[i])) {
			for (u16_t j = i + 1; j < len; j++) {
				if (isspace((int)str[j])) {
					shift++;
					continue;
				}

				if (shift > 0) {
					/* +1 for EOS */
					memmove(&str[i + 1],
						&str[j],
						len - shift + 1);
					//len - j + 1);          SUBS            R3, R5, R8     R3 = R5 - R8;
					len -= shift;
					shift = 0U;
				}
				break;
			}
		}
	}
}

//CVE-2020-10059


static bool start_coap_client(void)
{
	struct addrinfo *addr;
	struct addrinfo hints;
	int resolve_attempts = 10;
	int ret = -1;

	if (IS_ENABLED(CONFIG_NET_IPV6)) {
		hints.ai_family = AF_INET6;
		hints.ai_socktype = SOCK_STREAM;
	} else if (IS_ENABLED(CONFIG_NET_IPV4)) {
		hints.ai_family = AF_INET;
		hints.ai_socktype = SOCK_STREAM;
	}


	int verify = 0;
	sec_tag_t sec_list[] = { CA_CERTIFICATE_TAG };
	int protocol = IPPROTO_DTLS_1_2;
	char port[] = "5684";


	//omit..
	return true;
}




static void hci_cmd_done(u16_t opcode, u8_t status, struct net_buf *buf)
{
	return;
}


u8_t util_ones_count_get(u8_t *octets, u8_t octets_len)
{
	u8_t one_count = 0U;

	while (octets_len--) {
		u8_t bite;

		bite = *octets;
		while (bite) {
			bite &= (bite - 1);
			one_count++;
		}
		octets++;
	}

	return one_count;
}

//CVE-2020-10066
struct net_buf *net_buf_get(struct k_fifo *fifo, s32_t timeout){return NULL;}
static void send_cmd(void)
{
	struct net_buf *buf;
	int err;
	buf = net_buf_get(NULL, 0);
//omit..
	err = 1;
	if (err) {

		//k_sem_give(&bt_dev.ncmd_sem);
		hci_cmd_done(1, BT_HCI_ERR_UNSPECIFIED,
			     NULL);
		//omit..
	}
}


//CVE-2020-10069


void ull_slave_setup(memq_link_t *link, struct node_rx_hdr *rx,
		     struct node_rx_ftr *ftr, struct lll_conn *lll)
{
	u32_t conn_offset_us, conn_interval_us;
	u8_t ticker_id_adv, ticker_id_conn;
	u8_t peer_addr[BDADDR_SIZE];
	u32_t ticks_slot_overhead;
	u32_t ticks_slot_offset;
	struct pdu_adv *pdu_adv;
	struct ll_adv_set *adv;
	struct node_rx_cc *cc;
	struct ll_conn *conn;
	u32_t ticker_status;
	u8_t peer_addr_type;
	u16_t win_offset;
	u16_t timeout;
	u16_t interval;
	u8_t chan_sel;

	((struct lll_adv *)ftr->param)->conn = NULL;

	adv = ((struct lll_adv *)ftr->param)->hdr.parent;
	conn = lll->hdr.parent;

	/* Populate the slave context */
	pdu_adv = (void *)((struct node_rx_pdu *)rx)->pdu;
	memcpy(&lll->crc_init[0], &pdu_adv->connect_ind.crc_init[0], 3);
	memcpy(&lll->access_addr[0], &pdu_adv->connect_ind.access_addr[0], 4);
	memcpy(&lll->data_chan_map[0], &pdu_adv->connect_ind.chan_map[0],
	       sizeof(lll->data_chan_map));
	lll->data_chan_count = util_ones_count_get(&lll->data_chan_map[0],
			       sizeof(lll->data_chan_map));
						 
	//	if (lll->data_chan_count < 2) {
//		return;
//	}					 
						 						 
	lll->data_chan_hop = pdu_adv->connect_ind.hop;
	
	//	if ((lll->data_chan_hop < 5) || (lll->data_chan_hop > 16)) {
	//	return;
//	}
	
	interval = sys_le16_to_cpu(pdu_adv->connect_ind.interval);
	lll->interval = interval;
//omit..
}

//CVE-2020-10062
int packet_length_decode(struct buf_ctx *buf, u32_t *length)
{
	u8_t shift = 0U;
	u8_t bytes = 0U;

	*length = 0U;
	do {
		if (bytes > MQTT_MAX_LENGTH_BYTES) {
		//if (bytes >= MQTT_MAX_LENGTH_BYTES) {
			return -EINVAL;
		}
		bytes++;
	} while ((*(buf->cur++) & MQTT_LENGTH_CONTINUATION_BIT) != 0U);
//
//	if (*length > MQTT_MAX_PAYLOAD_SIZE) {
//		return -EINVAL;
//	}
//
	MQTT_TRC("length:0x%08x", *length);

	return 0;
}
//2020-10063


static int read_u8(u8_t *data, u16_t offset, u16_t *pos,
		   u16_t max_len, u8_t *value)
{
	if (max_len - offset < 1) {
		return -EINVAL;
	}
	*value = data[offset++];
	*pos = offset;
	return max_len - offset;
}
static u8_t option_header_get_delta(u8_t opt)
{
	return (opt & 0xF0) >> 4;
}
static u8_t option_header_get_len(u8_t opt)
{
	return opt & 0x0F;
}

static int decode_delta(u8_t *data, u16_t offset, u16_t *pos, u16_t max_len,
			u16_t opt, u16_t *opt_ext, u16_t *hdr_len)
{
	int ret = 0;

	return ret;
}

static inline bool u16_add_overflow(u16_t a, u16_t b, u16_t *result)
{
	u16_t c = a + b;

	*result = c;

	return c < a;
}

static int parse_option(u8_t *data, u16_t offset, u16_t *pos,
			u16_t max_len, u16_t *opt_delta, u16_t *opt_len,
			struct coap_option *option)
{
	u16_t hdr_len;
	u16_t delta;
	u16_t len;
	u8_t opt;
	int r;
	r = read_u8(data, offset, pos, max_len, &opt);
	if (r < 0) {
		return r;
	}
	*opt_len += 1U;
	if (opt == COAP_MARKER) {
		return r > 0 ? 0 : -EINVAL;
	}
	delta = option_header_get_delta(opt);
	len = option_header_get_len(opt);
	if (r == 0 && delta > COAP_OPTION_NO_EXT) {
		return -EINVAL;
	}
	if (delta > COAP_OPTION_NO_EXT) {
		r = decode_delta(data, *pos, pos, max_len,
				 delta, &delta, &hdr_len);
		if ((r < 0) || (r == 0 && len > COAP_OPTION_NO_EXT)) {
			return -EINVAL;
		}
		*opt_len += hdr_len;//
	//	if (u16_add_overflow(*opt_len, hdr_len, opt_len)) {
	//		return -EINVAL;
	//	}
	}
	if (len > COAP_OPTION_NO_EXT) {
			return -EINVAL;
		}
		*opt_len += hdr_len;//
	//	if (u16_add_overflow(*opt_len, hdr_len, opt_len)) {
	//		return -EINVAL;
	//	}
	
	*opt_delta += delta;//
	*opt_len += len;//
	//if (u16_add_overflow(*opt_delta, delta, opt_delta) ||
	 //  u16_add_overflow(*opt_len, len, opt_len)) {
	//	return -EINVAL;
	//}
	if (r == 0) {
		if (len == 0U) {

			return -EINVAL;
		}
	} else {
		*pos += len;//
	  //if (u16_add_overflow(*pos, len, pos)) {
		//	return -EINVAL;
		//}
		r = max_len - *pos;
	}
	return r;
}



//2020-10070

static int mqtt_read_message_chunk(struct mqtt_client *client,
				   struct buf_ctx *buf, u32_t length)
{
	int remaining;
//	u32_t remaining;
	int len;

	//if (length <= (buf->end - buf->cur)) {//
	//	return 0;//
	//}//

	remaining = length - (buf->end - buf->cur);
	if (remaining <= 0) {
		return 0;
	}

	if (buf->end + remaining > client->rx_buf + client->rx_buf_size) {
		MQTT_ERR("[CID %p]: Buffer too small to receive the message",
	//if ((buf->end + remaining > client->rx_buf + client->rx_buf_size) ||//
	 //   (buf->end + remaining < client->rx_buf)) {//
	//	MQTT_ERR("[CID %p]: Read would exceed RX buffer bounds.",//
			 client);
		return -ENOMEM;
	}
}


//CVE-2023-6881
int strcmp(const char *s1, const char *s2)
{
	while ((*s1 == *s2) && (*s1 != '\0')) {
		s1++;
		s2++;
	}
	return *s1 - *s2;
}

static bool is_mount_point(const char *path)
{
	char dir_path[PATH_MAX];
  //size_t len;
	sprintf(dir_path, "%s", path);
  //len = strlen(path);
	//if (len >=  sizeof(dir_path)) {
	//	return false;
  //}

	//memcpy(dir_path, path, len);
	//dir_path[len] = '\0';
	return strcmp("/", "/") == 0;
}








struct shell {
	const char *default_prompt; /*!< shell default prompt. */

	const char *thread_name;

};

void shell_fprintf(const struct shell *shell, enum shell_vt100_color color,
		   const char *fmt, ...)
{return;
}

static int settings_parse_type(const char *type, enum settings_value_types *value_type)
{
	if (strcmp(type, "string") == 0) {
		*value_type = SETTINGS_VALUE_STRING;
	} else if (strcmp(type, "hex") == 0) {
		*value_type = SETTINGS_VALUE_HEX;
	} else {
		return -EINVAL;
	}

	return 0;
}
int settings_save_one(const char *name, const void *value, size_t val_len)
{
	return 0;
}

size_t hex2bin(const char *hex, size_t hexlen, uint8_t *buf, size_t buflen)
{
	
	return hexlen / 2U + hexlen % 2U;
}




//CVE-2023-6749
static int cmd_write(const struct shell *shell_ptr, size_t argc, char *argv[])
{
	int err;
	uint8_t buffer[CONFIG_SHELL_CMD_BUFF_SIZE / 2];
	size_t buffer_len = 0;
	enum settings_value_types value_type = SETTINGS_VALUE_HEX;
	if (argc > 3) {
		err = settings_parse_type(argv[1], &value_type);
		if (err) {
			shell_error(shell_ptr, "Invalid type: %s", argv[1]);
			return err;
		}
	}
	switch (value_type) {
	case SETTINGS_VALUE_HEX:
		buffer_len = hex2bin(argv[argc - 1], strlen(argv[argc - 1]),
			buffer, sizeof(buffer));
		break;
	case SETTINGS_VALUE_STRING:
		buffer_len = strlen(argv[argc - 1]) + 1;
//		if (buffer_len > sizeof(buffer)) {//
//			shell_error(shell_ptr, "%s is bigger than shell's buffer", argv[argc - 1]);//
	//		return -EINVAL;//
//		}//
		memcpy(buffer, argv[argc - 1], buffer_len);
		break;
	}
	if (buffer_len == 0) {
		shell_error(shell_ptr, "Failed to parse value");
		return -EINVAL;
	}
	err = settings_save_one(argv[argc - 2], buffer, buffer_len);
	if (err) {
		shell_error(shell_ptr, "Failed to write setting: %d", err);
	}
	return err;
}

//2022-1841



static inline int snprintk(char *buf, unsigned long len, const char *f, ...)
{
	int ret;
//	struct _pfr r = { .buf = buf, .len = len };

//	CALL_VPF(&r);
	return ret;
}

static const char *tcp_flags(uint8_t flags)
{
#define BUF_SIZE 25 /* 6 * 4 + 1 */
	static char buf[BUF_SIZE];
	int len = 0;
	buf[0] = '\0';
	if (flags) {
		if (flags & SYN) {
			len += snprintk(buf + len, BUF_SIZE - len, "SYN,");
		}
		if (flags & FIN) {
			len += snprintk(buf + len, BUF_SIZE - len, "FIN,");
		}
		if (flags & ACK) {
			len += snprintk(buf + len, BUF_SIZE - len, "ACK,");
		}
		if (flags & PSH) {
			len += snprintk(buf + len, BUF_SIZE - len, "PSH,");
		}
		if (flags & RST) {
			len += snprintk(buf + len, BUF_SIZE - len, "RST,");
		}
		if (flags & URG) {
			len += snprintk(buf + len, BUF_SIZE - len, "URG,");
		}

		buf[len - 1] = '\0'; /* delete the last comma */
	//	if (len > 0) {
	//		buf[len - 1] = '\0'; /* delete the last comma */
	//	}
	}
#undef BUF_SIZE
	return buf;
}

//2021-3581

uint8_t ull_scan_rsp_set(struct ll_adv_set *adv, uint8_t len,
			 uint8_t const *const data)
{
	struct pdu_adv *prev;
	struct pdu_adv *pdu;
	uint8_t idx;

//	if (len > PDU_AC_DATA_SIZE_MAX) {//
//		return BT_HCI_ERR_INVALID_PARAM;//
//	}//

	/* update scan pdu fields. */
	//omit...
	return 0;
}



//CVE-2023-5184
static int send(const struct device *dev, int wait, uint32_t id,
		const void *data, int size)
{
	//omit....

	if (size > MAX_MSG) {
//	if ((size < 0) || (size > MAX_MSG)) {
		return -EMSGSIZE;
	}

	if ((id & 0xc0000000) != 0) {
		/* cAVS IDR register has only 30 usable bits */
		return -EINVAL;
	}
	uint32_t ext_data = 0;
	/* Protocol variant (used by SOF "ipc4"): store the first word
	 * of the message in the IPC scratch registers
	 */
	if (IS_ENABLED(CONFIG_IPM_CAVS_HOST_REGWORD) && size >= 4) {
		ext_data = ((uint32_t *)data)[0];
		data = &((const uint32_t *)data)[1];
		size -= 4;
	}
	//omit...
	return 1;
}


void k_sem_reset(struct k_sem *sem){return;}
//CVE-2023-4263

static int nrf5_tx(const struct device *dev,
		   enum ieee802154_tx_mode mode,
		   struct net_pkt *pkt,
		   struct net_buf *frag)
{
	struct nrf5_802154_data *nrf5_radio = NRF5_802154_DATA(dev);
	uint8_t payload_len = frag->len;
	uint8_t *payload = frag->data;
	bool ret = true;

	//if (payload_len > NRF5_PSDU_LENGTH) {
	//	return -EMSGSIZE;
	//}
	
	nrf5_radio->tx_psdu[0] = payload_len + NRF5_FCS_LENGTH;
	memcpy(nrf5_radio->tx_psdu + 1, payload, payload_len);
	/* Reset semaphore in case ACK was received after timeout */
	k_sem_reset(&nrf5_radio->tx_wait);
	//omit...
	return 0;
}

// test for unbounded loop
// CVE_2020_17445

int pico_ipv6_process_destopt(uint8_t *destopt, uint8_t *f, uint32_t opt_ptr)
{
    uint8_t *option = NULL;
    uint8_t len = 0, optlen = 0;
    option = (destopt + 2); /* Skip Dest_opts header */
    len = (uint8_t)(((*(destopt + 1) + 1) << 3) - 2); /* len in bytes, minus nxthdr and len byte */
  //  if((optlen > len ) || (optlen == 0))
		//	return -1;
	  while (len) {
        optlen = (uint8_t)(*(option + 1) + 2);
			
        switch (*option)
        {
        case PICO_IPV6_EXTHDR_OPT_PAD1:
            break;

        case PICO_IPV6_EXTHDR_OPT_PADN:
            break;

        case PICO_IPV6_EXTHDR_OPT_SRCADDR:
            // ipv6_dbg("IPv6: home address option with length %u\n", optlen);
            break;

        default:
            // THE CODE HERE IS OMITTED FOR BREVITY
            break;
        }
        opt_ptr += optlen;
        option += optlen;
        len = (uint8_t)(len - optlen);
    }
    return 0;
}



//CVE-2020-24337

static int tcp_parse_options(struct pico_frame *f)
{
    struct pico_socket_tcp *t = (struct pico_socket_tcp *)f->sock;
    uint8_t *opt = f->transport_hdr + PICO_SIZE_TCPHDR;
    uint32_t i = 0;
    f->timestamp = 0;

   // if (f->transport_hdr + f->transport_len > f->buffer + f->buffer_len)
   //     return -1;

    while (i < (f->transport_len - PICO_SIZE_TCPHDR)) {
        uint8_t type =  opt[i++];
        uint8_t len;
        if(i < (f->transport_len - PICO_SIZE_TCPHDR) && (type > 1))
            len =  opt[i++];
        else
            len = 1;

        if (f->payload && ((opt + i) > f->payload))
            break;

        if (len == 0) {
            return -1;
        }

       
        switch (type) {
        case PICO_TCP_OPTION_NOOP:
        case PICO_TCP_OPTION_END:
            break;
        case PICO_TCP_OPTION_WS:
           // tcp_parse_option_ws(t, len, opt, &i);
            break;
        case PICO_TCP_OPTION_SACK_OK:
           // tcp_parse_option_sack_ok(t, f, len, &i);
            break;
        case PICO_TCP_OPTION_MSS:
           // tcp_parse_option_mss(t, len, opt, &i);
            break;
        case PICO_TCP_OPTION_TIMESTAMP:
           // tcp_parse_option_timestamp(t, f, len, opt, &i);
            break;

        case PICO_TCP_OPTION_SACK:
       //     tcp_rcv_sack(t, opt + i, len - 2);
            i = i + len - 2;
            break;
        default:
            
            i = i + len - 2;
        }
    }
    return 0;
}
//CVE-2024-0901

int ProcessReplyEx(WOLFSSL* ssl, int allowSocketErr)
{
	if (ssl->options.tls1_3) {
      word32 i = (ssl->buffers.inputBuffer.idx +
      ssl->curSize - ssl->specs.aead_mac_size);

      if (i > ssl->buffers.inputBuffer.length) {
            return BUFFER_ERROR;
      }
      // if (i > ssl->buffers.inputBuffer.length || i == 0) {
      //    return BUFFER_ERROR;
      //}
      for (--i; i > ssl->buffers.inputBuffer.idx; i--) {
          if (ssl->buffers.inputBuffer.buffer[i] != 0)
          break;
          }

      ssl->curRL.type = ssl->buffers.inputBuffer.buffer[i];
      ssl->keys.padSz = ssl->buffers.inputBuffer.idx
                      + ssl->curSize - i;
          }
}




                