#include "stm32f4xx.h"
#include "usart.h"

enum dns_resolve_status {
	DNS_EAI_BADFLAGS    = -1,   /* Invalid value for `ai_flags' field */
	DNS_EAI_NONAME      = -2,   /* NAME or SERVICE is unknown */
	DNS_EAI_AGAIN       = -3,   /* Temporary failure in name resolution */
	DNS_EAI_FAIL        = -4,   /* Non-recoverable failure in name res */
	DNS_EAI_NODATA      = -5,   /* No address associated with NAME */
	DNS_EAI_FAMILY      = -6,   /* `ai_family' not supported */
	DNS_EAI_SOCKTYPE    = -7,   /* `ai_socktype' not supported */
	DNS_EAI_SERVICE     = -8,   /* SRV not supported for `ai_socktype' */
	DNS_EAI_ADDRFAMILY  = -9,   /* Address family for NAME not supported */
	DNS_EAI_MEMORY      = -10,  /* Memory allocation failure */
	DNS_EAI_SYSTEM      = -11,  /* System error returned in `errno' */
	DNS_EAI_OVERFLOW    = -12,  /* Argument buffer overflow */
	DNS_EAI_INPROGRESS  = -100, /* Processing request in progress */
	DNS_EAI_CANCELED    = -101, /* Request canceled */
	DNS_EAI_NOTCANCELED = -102, /* Request not canceled */
	DNS_EAI_ALLDONE     = -103, /* All requests done */
	DNS_EAI_IDN_ENCODE  = -105, /* IDN encoding failed */
};


#define _u8  unsigned char
#define _i8  signed char
#define _u16 unsigned short
#define _i16 signed short
#define _u32 unsigned long
#define _i32 signed long

typedef signed char         s8_t;
typedef signed short        s16_t;
typedef signed int          s32_t;
typedef signed long long    s64_t;

typedef unsigned char       u8_t;
typedef unsigned short      u16_t;
typedef unsigned int        u32_t;
typedef unsigned long long  u64_t;

#define _volatile volatile
#define _const    const
typedef unsigned short int sa_family_t;
typedef struct SlInAddr_t
{
#ifndef s_addr
    _u32           s_addr;             /* Internet address 32 bits */
#else
    union S_un {
       struct { _u8 s_b1,s_b2,s_b3,s_b4; } S_un_b;
       struct { _u16 s_w1,s_w2; } S_un_w;
        _u32 S_addr;
    } S_un;
#endif
}SlInAddr_t;
#define in_addr                             SlInAddr_t
struct sockaddr_in {
	sa_family_t		sin_family;    /* AF_INET      */
	u16_t			sin_port;      /* Port number  */
	struct in_addr		sin_addr;      /* IPv4 address */
};

#define NET_SOCKADDR_MAX_SIZE (sizeof(struct sockaddr_in))
struct sockaddr {
	sa_family_t sa_family;
	char data[NET_SOCKADDR_MAX_SIZE - sizeof(sa_family_t)];
};

typedef _u16 SlSocklen_t;
#define socklen_t                           SlSocklen_t


#define DNS_MAX_NAME_SIZE 20
struct zsock_addrinfo {
	struct zsock_addrinfo *ai_next;
	int ai_flags;
	int ai_family;
	int ai_socktype;
	int ai_protocol;
	socklen_t ai_addrlen;
	struct sockaddr *ai_addr;
	char *ai_canonname;

	struct sockaddr _ai_addr;
	char _ai_canonname[DNS_MAX_NAME_SIZE + 1];
};
struct dns_addrinfo {
	struct sockaddr ai_addr;
	socklen_t       ai_addrlen;
	u8_t            ai_family;
	char            ai_canonname[DNS_MAX_NAME_SIZE + 1];
};


struct _dnode {
	union {
		struct _dnode *head; /* ptr to head of list (sys_dlist_t) */
		struct _dnode *next; /* ptr to next node    (sys_dnode_t) */
	};
	union {
		struct _dnode *tail; /* ptr to tail of list (sys_dlist_t) */
		struct _dnode *prev; /* ptr to previous node (sys_dnode_t) */
	};
};
#ifdef CONFIG_POLL
#define _POLL_EVENT_OBJ_INIT(obj) \
	.poll_events = SYS_DLIST_STATIC_INIT(&obj.poll_events),
#define _POLL_EVENT sys_dlist_t poll_events
#else
#define _POLL_EVENT_OBJ_INIT(obj)
#define _POLL_EVENT
#endif
#define _OBJECT_TRACING_NEXT_PTR(type)
typedef struct _dnode sys_dlist_t;
typedef sys_dlist_t _wait_q_t;

struct k_sem {
	_wait_q_t wait_q;
	unsigned int count;
	unsigned int limit;
	_POLL_EVENT;
	_OBJECT_TRACING_NEXT_PTR(k_sem);
};

struct getaddrinfo_state {
	const struct zsock_addrinfo *hints;
	struct k_sem sem;
	int status;
	u16_t idx;
	u16_t port;
};
#define SL_SOCK_STREAM                         (1)         
#define SOCK_STREAM                         SL_SOCK_STREAM
#define NET_DBG(...)
//#define NULL (( void * )( 0x0UL ))
static struct zsock_addrinfo ai_arr[2];
#define SL_IPPROTO_TCP                         (6)                      
#define IPPROTO_TCP                         SL_IPPROTO_TCP  /* tcp */
#define SL_SOCK_DGRAM                          (2)                      
#define SOCK_DGRAM                          SL_SOCK_DGRAM
#define SL_IPPROTO_UDP                         (17)
#define IPPROTO_UDP                         SL_IPPROTO_UDP  /* user datagram protocol */

#define _MLIBC_RESTRICT restrict
#ifndef ZTEST_UNITTEST
#define __syscall static inline
#else
#define __syscall
#endif /* #ifndef ZTEST_UNITTEST */

#define ZERO_OR_COMPILE_ERROR(cond) ((int) sizeof(char[1 - 2 * !(cond)]) - 1)
#define IS_ARRAY(array) \
	ZERO_OR_COMPILE_ERROR( \
		!__builtin_types_compatible_p(__typeof__(array), \
					      __typeof__(&(array)[0])))

#define ARRAY_SIZE(array) \
	((unsigned long) (IS_ARRAY(array) + \
		(sizeof(array) / sizeof((array)[0]))))
struct usb_setup_packet {
	u8_t bmRequestType;  /**< characteristics of the specific request */
	u8_t bRequest;       /**< specific request */
	u16_t wValue;        /**< request specific parameter */
	u16_t wIndex;        /**< request specific parameter */
	u16_t wLength;       /**< length of data transferred in data phase */
};

#define DFU_DETACH			0x00
#define DFU_DNLOAD			0x01
#define DFU_UPLOAD			0x02
#define DFU_GETSTATUS			0x03
#define DFU_CLRSTATUS			0x04
#define DFU_GETSTATE			0x05
#define DFU_ABORT			0x06


#define USB_DFU_MAX_XFER_SIZE		64


struct flash_img_context {
	u8_t buf[512];
	const struct flash_area *flash_area;
	size_t bytes_written;
	u16_t buf_bytes;
};
enum dfu_state {
	appIDLE,
	appDETACH,
	dfuIDLE,
	dfuDNLOAD_SYNC,
	dfuDNBUSY,
	dfuDNLOAD_IDLE,
	dfuMANIFEST_SYNC,
	dfuMANIFEST,
	dfuMANIFEST_WAIT_RST,
	dfuUPLOAD_IDLE,
	dfuERROR,
};
enum dfu_status {
	statusOK,
	errTARGET,
	errFILE,
	errWRITE,
	errERASE,
	errCHECK_ERASED,
	errPROG,
	errVERIFY,
	errADDRESS,
	errNOTDONE,
	errFIRMWARE,
	errVENDOR,
	errUSB,
	errPOR,
	errUNKNOWN,
	errSTALLEDPKT
};
struct dfu_data_t {
	u8_t flash_area_id;
	u32_t flash_upload_size;
	/* Number of bytes sent during upload */
	u32_t bytes_sent;
	u32_t alt_setting;              /* DFU alternate setting */
	u8_t buffer[USB_DFU_MAX_XFER_SIZE]; /* DFU data buffer */
	struct flash_img_context ctx;
	enum dfu_state state;              /* State of the DFU device */
	enum dfu_status status;            /* Status of the DFU device */
	u16_t block_nr;                 /* DFU block number */
	u16_t bwPollTimeout;
};

#define __packed __attribute__((__packed__))
struct CBW {
	u32_t Signature;
	u32_t Tag;
	u32_t DataLength;
	u8_t  Flags;
	u8_t  LUN;
	u8_t  CBLength;
	u8_t  CB[16];
} __packed;

struct CSW {
	u32_t Signature;
	u32_t Tag;
	u32_t DataResidue;
	u8_t  Status;
} __packed;
static struct CBW cbw;
static struct CSW csw;
static u32_t addr;
enum Status {
	CSW_PASSED,
	CSW_FAILED,
	CSW_ERROR,
};
#define BLOCK_SIZE	512
static u32_t memory_size;
#define CSW_Signature			0x53425355

#define IS_ENABLED(config_macro) Z_IS_ENABLED1(config_macro)
#define Z_IS_ENABLED1(config_macro) Z_IS_ENABLED2(_XXXX##config_macro)
#define Z_IS_ENABLED2(one_or_two_args) Z_IS_ENABLED3(one_or_two_args true, false)
#define Z_IS_ENABLED3(ignore_this, val, ...) val
#define PF_INET         1          /**< IP protocol family version 4. */
#define PF_INET6        2          /**< IP protocol family version 6. */
#define AF_INET        PF_INET     /**< IP protocol family version 4. */
#define AF_INET6       PF_INET6    /**< IP protocol family version 6. */
#define addrinfo zsock_addrinfo

#define CA_CERTIFICATE_TAG 1
typedef int sec_tag_t;
enum net_ip_protocol_secure {
	IPPROTO_TLS_1_0 = 256,     /**< TLS 1.0 protocol */
	IPPROTO_TLS_1_1 = 257,     /**< TLS 1.1 protocol */
	IPPROTO_TLS_1_2 = 258,     /**< TLS 1.2 protocol */
	IPPROTO_DTLS_1_0 = 272,    /**< DTLS 1.0 protocol */
	IPPROTO_DTLS_1_2 = 273,    /**< DTLS 1.2 protocol */
};

#define BT_HCI_ERR_UNSPECIFIED                  0x1f
static struct dfu_data_t dfu_data = {
	.state = appIDLE,
	.status = statusOK,
//	.flash_area_id = DT_FLASH_AREA_IMAGE_1_ID,
	//.alt_setting = 0,
	//.bwPollTimeout = CONFIG_USB_DFU_DEFAULT_POLLTIMEOUT,
};


struct net_buf {
	union {
		/** Allow placing the buffer into sys_slist_t */


		/** Fragments associated with this buffer. */
		struct net_buf *frags;
	};

	/** Reference count. */
	u8_t ref;

	/** Bit-field of buffer flags. */
	u8_t flags;

	/** Where the buffer should go when freed up. */
	u8_t pool_id;

	/* Union for convenience access to the net_buf_simple members, also
	 * preserving the old API.
	 */
	union {
		/* The ABI of this struct must match net_buf_simple */
		struct {
			/** Pointer to the start of data in the buffer. */
			u8_t *data;

			/** Length of the data behind the data pointer. */
			u16_t len;

			/** Amount of data that this buffer can store. */
			u16_t size;

			/** Start of the data storage. Not to be accessed
			 *  directly (the data pointer should be used
			 *  instead).
			 */
			u8_t *__buf;
		};

		
	};


};
struct k_queue {
	
	_OBJECT_TRACING_NEXT_PTR(k_queue)
	
};
struct k_fifo {
	struct k_queue _queue;
};
static inline struct sockaddr_in *net_sin(const struct sockaddr *addr)
{
	return (struct sockaddr_in *)addr;
}

void dns_resolve_cb(enum dns_resolve_status status,
			   struct dns_addrinfo *info, void *user_data);

struct _memq_link {
	struct _memq_link *next; /* permit chaining */
	void              *mem;  /* payload */
};

typedef struct _memq_link memq_link_t;
struct node_rx_ftr {
	void  *param;
	void  *extra;
	u32_t ticks_anchor;
	u32_t us_radio_end;
	u32_t us_radio_rdy;
	u8_t  rssi;
#if defined(CONFIG_BT_CTLR_PRIVACY)
	u8_t  rl_idx;
#endif /* CONFIG_BT_CTLR_PRIVACY */
#if defined(CONFIG_BT_CTLR_EXT_SCAN_FP)
	u8_t  direct;
#endif /* CONFIG_BT_CTLR_EXT_SCAN_FP */
#if defined(CONFIG_BT_HCI_MESH_EXT)
	u8_t  chan_idx;
#endif /* CONFIG_BT_HCI_MESH_EXT */
};
enum node_rx_type {
	/* Unused */
	NODE_RX_TYPE_NONE = 0x00,
	/* Signals completion of RX event */
	NODE_RX_TYPE_EVENT_DONE = 0x01,
	/* Signals arrival of RX Data Channel payload */
	NODE_RX_TYPE_DC_PDU = 0x02,
	/* Signals release of RX Data Channel payload */
	NODE_RX_TYPE_DC_PDU_RELEASE = 0x03,

#if defined(CONFIG_BT_OBSERVER)
	/* Advertisement report from scanning */
	NODE_RX_TYPE_REPORT = 0x04,
#endif /* CONFIG_BT_OBSERVER */

#if defined(CONFIG_BT_CTLR_ADV_EXT)
	NODE_RX_TYPE_EXT_1M_REPORT = 0x05,
	NODE_RX_TYPE_EXT_CODED_REPORT = 0x06,
#endif /* CONFIG_BT_CTLR_ADV_EXT */

#if defined(CONFIG_BT_CTLR_SCAN_REQ_NOTIFY)
	NODE_RX_TYPE_SCAN_REQ = 0x07,
#endif /* CONFIG_BT_CTLR_SCAN_REQ_NOTIFY */

#if defined(CONFIG_BT_CONN)
	NODE_RX_TYPE_CONNECTION = 0x08,
	NODE_RX_TYPE_TERMINATE = 0x09,
	NODE_RX_TYPE_CONN_UPDATE = 0x0A,
	NODE_RX_TYPE_ENC_REFRESH = 0x0B,

#if defined(CONFIG_BT_CTLR_LE_PING)
	NODE_RX_TYPE_APTO = 0x0C,
#endif /* CONFIG_BT_CTLR_LE_PING */

	NODE_RX_TYPE_CHAN_SEL_ALGO = 0x0D,

#if defined(CONFIG_BT_CTLR_PHY)
	NODE_RX_TYPE_PHY_UPDATE = 0x0E,
#endif /* CONFIG_BT_CTLR_PHY */

#if defined(CONFIG_BT_CTLR_CONN_RSSI)
	NODE_RX_TYPE_RSSI = 0x0F,
#endif /* CONFIG_BT_CTLR_CONN_RSSI */
#endif /* CONFIG_BT_CONN */

#if defined(CONFIG_BT_CTLR_PROFILE_ISR)
	NODE_RX_TYPE_PROFILE = 0x10,
#endif /* CONFIG_BT_CTLR_PROFILE_ISR */

#if defined(CONFIG_BT_CTLR_ADV_INDICATION)
	NODE_RX_TYPE_ADV_INDICATION = 0x11,
#endif /* CONFIG_BT_CTLR_ADV_INDICATION */

#if defined(CONFIG_BT_CTLR_SCAN_INDICATION)
	NODE_RX_TYPE_SCAN_INDICATION = 0x12,
#endif /* CONFIG_BT_CTLR_SCAN_INDICATION */

#if defined(CONFIG_BT_HCI_MESH_EXT)
	NODE_RX_TYPE_MESH_ADV_CPLT = 0x13,
	NODE_RX_TYPE_MESH_REPORT = 0x14,
#endif /* CONFIG_BT_HCI_MESH_EXT */

/* Following proprietary defines must be at end of enum range */
#if defined(CONFIG_BT_CTLR_USER_EXT)
	NODE_RX_TYPE_USER_START = 0x15,
	NODE_RX_TYPE_USER_END = NODE_RX_TYPE_USER_START +
				CONFIG_BT_CTLR_USER_EVT_RANGE,
#endif /* CONFIG_BT_CTLR_USER_EXT */

};
struct node_rx_hdr {
	union {
		void        *next;
		memq_link_t *link;
		u8_t        ack_last;
	};

	enum node_rx_type   type;
	u8_t                user_meta; /* User metadata */
	u16_t               handle;

	union {
#if defined(CONFIG_BT_CTLR_RX_PDU_META)
		lll_rx_pdu_meta_t  rx_pdu_meta;
#endif /* CONFIG_BT_CTLR_RX_PDU_META */
		struct node_rx_ftr rx_ftr;
	};
};
struct lll_hdr {
	void *parent;
	u8_t is_stop:1;
};
struct lll_conn {
  struct lll_hdr hdr;
	u8_t access_addr[4];
	u8_t crc_init[3];

	u16_t handle;
	u16_t interval;
	u16_t latency;

	u16_t latency_prepare;
	u16_t latency_event;
	u16_t event_counter;

	u8_t data_chan_map[5];
	u8_t data_chan_count:6;
	u8_t data_chan_sel:1;
	u8_t role:1;

	union {
		struct {
			u8_t data_chan_hop;
			u8_t data_chan_use;
		};

		u16_t data_chan_id;
	};

#if defined(CONFIG_BT_PERIPHERAL)
	struct {
		u8_t  latency_enabled:1;
		u32_t window_widening_periodic_us;
		u32_t window_widening_max_us;
		u32_t window_widening_prepare_us;
		u32_t window_widening_event_us;
		u32_t window_size_prepare_us;
		u32_t window_size_event_us;
	} slave;
#endif /* CONFIG_BT_PERIPHERAL */

#if defined(CONFIG_BT_CTLR_DATA_LENGTH)
	u16_t max_tx_octets;
	u16_t max_rx_octets;

#if defined(CONFIG_BT_CTLR_PHY)
	u16_t max_tx_time;
	u16_t max_rx_time;
#endif /* CONFIG_BT_CTLR_PHY */
#endif /* CONFIG_BT_CTLR_DATA_LENGTH */

#if defined(CONFIG_BT_CTLR_PHY)
	u8_t phy_tx:3;
	u8_t phy_flags:1;
	u8_t phy_tx_time:3;
	u8_t phy_rx:3;
#endif /* CONFIG_BT_CTLR_PHY */

	
	memq_link_t link_tx;
	memq_link_t *link_tx_free;
	u8_t  packet_tx_head_len;
	u8_t  packet_tx_head_offset;

	u8_t sn:1;
	u8_t nesn:1;
	u8_t empty:1;

#if defined(CONFIG_BT_CTLR_LE_ENC)
	u8_t enc_rx:1;
	u8_t enc_tx:1;

	struct ccm ccm_rx;
	struct ccm ccm_tx;
#endif /* CONFIG_BT_CTLR_LE_ENC */

#if defined(CONFIG_BT_CTLR_CONN_RSSI)
	u8_t  rssi_latest;
	u8_t  rssi_reported;
	u8_t  rssi_sample_count;
#endif /* CONFIG_BT_CTLR_CONN_RSSI */

#if defined(CONFIG_BT_CTLR_CONN_META)
	struct lll_conn_meta conn_meta;
#endif /* CONFIG_BT_CTLR_CONN_META */

#if defined(CONFIG_BT_CTLR_TX_PWR_DYNAMIC_CONTROL)
	s8_t tx_pwr_lvl;
#endif
};
#define MQTT_ERR printf

struct lll_adv {


   struct lll_hdr hdr;
	/* NOTE: conn context has to be after lll_hdr */
	struct lll_conn *conn;
	u8_t is_hdcd:1;

	u8_t chan_map:3;
	u8_t chan_map_curr:3;
	u8_t filter_policy:2;

#if defined(CONFIG_BT_CTLR_ADV_EXT)
	u8_t phy_p:3;
#endif /* !CONFIG_BT_CTLR_ADV_EXT */

#if defined(CONFIG_BT_HCI_MESH_EXT)
	u8_t is_mesh:1;
#endif /* CONFIG_BT_HCI_MESH_EXT */

#if defined(CONFIG_BT_CTLR_PRIVACY)
	u8_t  rl_idx;
#endif /* CONFIG_BT_CTLR_PRIVACY */


#if defined(CONFIG_BT_CTLR_TX_PWR_DYNAMIC_CONTROL)
	s8_t tx_pwr_lvl;
#endif /* CONFIG_BT_CTLR_TX_PWR_DYNAMIC_CONTROL */
};
#define BDADDR_SIZE 6

struct node_rx_pdu {
	struct node_rx_hdr hdr;
	u8_t               pdu[0];
};

struct pdu_adv_connect_ind {
	u8_t init_addr[BDADDR_SIZE];
	u8_t adv_addr[BDADDR_SIZE];
	struct {
		u8_t  access_addr[4];
		u8_t  crc_init[3];
		u8_t  win_size;
		u16_t win_offset;
		u16_t interval;
		u16_t latency;
		u16_t timeout;
		u8_t  chan_map[5];
#if __BYTE_ORDER__ == __ORDER_LITTLE_ENDIAN__
		u8_t  hop:5;
		u8_t  sca:3;
#elif __BYTE_ORDER__ == __ORDER_BIG_ENDIAN__
		u8_t  sca:3;
		u8_t  hop:5;
#else
#error "Unsupported endianness"
#endif

	} __packed;
} __packed;

struct pdu_adv {
#if __BYTE_ORDER__ == __ORDER_LITTLE_ENDIAN__
	u8_t type:4;
	u8_t rfu:1;
	u8_t chan_sel:1;
	u8_t tx_addr:1;
	u8_t rx_addr:1;
#elif __BYTE_ORDER__ == __ORDER_BIG_ENDIAN__
	u8_t rx_addr:1;
	u8_t tx_addr:1;
	u8_t chan_sel:1;
	u8_t rfu:1;
	u8_t type:4;
#else
#error "Unsupported endianness"
#endif

	u8_t len;

	union {
		u8_t   payload[0];
		
		struct pdu_adv_connect_ind connect_ind;

#if defined(CONFIG_BT_CTLR_ADV_EXT)
		struct pdu_adv_com_ext_adv adv_ext_ind;
#endif /* CONFIG_BT_CTLR_ADV_EXT */
	} __packed;
} __packed;



#define sys_le16_to_cpu(val) (val)
struct buf_ctx {
	u8_t *cur;
	u8_t *end;
};
#define MQTT_MAX_LENGTH_BYTES 4
#define MQTT_LENGTH_CONTINUATION_BIT 0x80
#define EINVAL 22 
#define MQTT_MAX_PAYLOAD_SIZE 0x0FFFFFFF
#define MQTT_TRC printf
struct coap_option {
	u16_t delta;
#if defined(CONFIG_COAP_EXTENDED_OPTIONS_LEN)
	u16_t len;
	u8_t value[CONFIG_COAP_EXTENDED_OPTIONS_LEN_VALUE];
#else
	u8_t len;
	u8_t value[12];
#endif
};
#define COAP_MARKER		0xFF
#define COAP_OPTION_NO_EXT 12
struct mqtt_client {
	

	u8_t *rx_buf;

	/** Size of receive buffer. */
	u32_t rx_buf_size;

	/** Transmit buffer used for creating MQTT packet in TX path. */
	u8_t *tx_buf;

	/** Size of transmit buffer. */
	u32_t tx_buf_size;

	/** Keepalive interval for this client in seconds.
	 *  Default is CONFIG_MQTT_KEEPALIVE.
	 */
	u16_t keepalive;

	/** MQTT protocol version. */
	u8_t protocol_version;

	/** Unanswered PINGREQ count on this connection. */
	s8_t unacked_ping;

	
	u8_t will_retain : 1;

	u8_t clean_session : 1;
};

#define ENOMEM 12 
#define PATH_MAX    256
enum shell_vt100_color {
	SHELL_VT100_COLOR_DEFAULT,
	SHELL_VT100_COLOR_BLACK,
	SHELL_VT100_COLOR_RED,
	SHELL_VT100_COLOR_GREEN,
	SHELL_VT100_COLOR_YELLOW,
	SHELL_VT100_COLOR_BLUE,
	SHELL_VT100_COLOR_MAGENTA,
	SHELL_VT100_COLOR_CYAN,
	SHELL_VT100_COLOR_WHITE,

	VT100_COLOR_END
};


#define SHELL_ERROR	SHELL_VT100_COLOR_RED
#define shell_error(_sh, _ft, ...) \
	shell_fprintf(_sh, SHELL_ERROR, _ft "\n", ##__VA_ARGS__)

#define CONFIG_SHELL_CMD_BUFF_SIZE 10
enum settings_value_types{
SETTINGS_VALUE_HEX,
SETTINGS_VALUE_STRING,
};
#define BIT(n)  (1UL << (n))
enum th_flags {
	FIN = BIT(0),
	SYN = BIT(1),
	RST = BIT(2),
	PSH = BIT(3),
	ACK = BIT(4),
	URG = BIT(5),
	ECN = BIT(6),
	CWR = BIT(7),
};
struct ll_adv_set {
	uint16_t interval;

	uint8_t is_enabled:1;

};
#define BT_HCI_ERR_INVALID_PARAM                0x12

#define PDU_AC_DATA_SIZE_MAX 64


struct device {
const char *name;const void *config;
      const void *api;
      void * const data;
};
#define EMSGSIZE 122  
#define MAX_MSG 384


#define IEEE802154_MAX_PHY_PACKET_SIZE	127
#define NRF5_PHR_LENGTH   (1)
enum ieee802154_tx_mode {
	/** Transmit packet immediately, no CCA. */
	IEEE802154_TX_MODE_DIRECT,

	/** Perform CCA before packet transmission. */
	IEEE802154_TX_MODE_CCA,

	/**
	 * Perform full CSMA/CA procedure before packet transmission.
	 *
	 * @note requires IEEE802154_HW_CSMA capability.
	 */
	IEEE802154_TX_MODE_CSMA_CA,

	/**
	 * Transmit packet in the future, at the specified time, no CCA.
	 *
	 * @note requires IEEE802154_HW_TXTIME capability.
	 */
	IEEE802154_TX_MODE_TXTIME,

	/**
	 * Transmit packet in the future, perform CCA before transmission.
	 *
	 * @note requires IEEE802154_HW_TXTIME capability.
	 *
	 * @note Required for Thread 1.2 Coordinated Sampled Listening feature
	 * (see Thread specification 1.2.0, ch. 3.2.6.3).
	 */
	IEEE802154_TX_MODE_TXTIME_CCA,

	/** Number of modes defined in ieee802154_tx_mode. */
	IEEE802154_TX_MODE_COMMON_COUNT,

	/** This and higher values are specific to the protocol- or driver-specific extensions. */
	IEEE802154_TX_MODE_PRIV_START = IEEE802154_TX_MODE_COMMON_COUNT,
};
struct net_pkt {
	uint16_t ll_proto_type;
};
struct nrf5_802154_data {
uint8_t mac[8];
uint8_t tx_psdu[NRF5_PHR_LENGTH + IEEE802154_MAX_PHY_PACKET_SIZE];
struct k_sem tx_wait;
};
#define NRF5_802154_DATA(dev) \
	((struct nrf5_802154_data * const)(dev)->data)
#define NRF5_PSDU_LENGTH 10
#define NRF5_FCS_LENGTH 5
	#define PICO_IPV6_EXTHDR_OPT_PAD1 0
#define PICO_IPV6_EXTHDR_OPT_PADN 1
#define PICO_IPV6_EXTHDR_OPT_SRCADDR 201

#define MAX_DEVICE_NAME 16
struct pico_device {
    char name[MAX_DEVICE_NAME];
    uint32_t hash;
    uint32_t overhead;
    uint32_t mtu;
 //   struct pico_ethdev *eth; /* Null if non-ethernet */
  //  enum pico_ll_mode mode;
   // struct pico_queue *q_in;
  //  struct pico_queue *q_out;
   // int (*link_state)(struct pico_device *self);
  //  int (*send)(struct pico_device *self, void *buf, int len); /* Send function. Return 0 if busy */
   // int (*poll)(struct pico_device *self, int loop_score);
  //  void (*destroy)(struct pico_device *self);
  #ifdef PICO_SUPPORT_TICKLESS
    void (*wfi)(struct pico_device *self, int timeout);
  #endif
   // int (*dsr)(struct pico_device *self, int loop_score);
   // int __serving_interrupt;
    /* used to signal the upper layer the number of events arrived since the last processing */
    volatile int eventCnt;
  #ifdef PICO_SUPPORT_IPV6
    struct pico_nd_hostvars hostvars;
  #endif
  //  struct pico_stack *stack;
};
typedef uint64_t pico_time;
struct pico_frame {

    /* Connector for queues */
    struct pico_frame *next;
    unsigned char *buffer;
    uint32_t buffer_len;
    unsigned char *start;
    uint32_t len;
    uint32_t *usage_count;
    uint8_t *datalink_hdr;
    uint8_t *net_hdr;
    uint16_t net_len;
    uint8_t *transport_hdr;
    uint16_t transport_len;
    uint8_t *app_hdr;
    uint16_t app_len;

    struct pico_device *dev;
    pico_time timestamp;
    /* Failures due to bad datalink addressing. */
    uint16_t failure_count;
    /* Protocol over IP */
    uint8_t proto;
    /* PICO_FRAME_FLAG_* */
    uint8_t flags;
    /* Pointer to payload */
    unsigned char *payload;
    uint16_t payload_len;
#if defined(PICO_SUPPORT_IPV4FRAG) || defined(PICO_SUPPORT_IPV6FRAG)
    /* Payload fragmentation info */
    uint16_t frag;
#endif

#if defined(PICO_SUPPORT_6LOWPAN)
    uint32_t hash;
    union pico_ll_addr src;
    union pico_ll_addr dst;
#endif

    struct pico_socket *sock;
    void *info;
    int8_t priority;
    uint8_t transport_flags_saved;

    void (*notify_free)(uint8_t *);

    uint8_t send_ttl; /* Special TTL/HOPS value, 0 = auto assign */
    uint8_t send_tos; /* Type of service */
};
// CVE_2020_17443
struct pico_tcp_hdr {
   // struct pico_trans trans;
    uint32_t seq;
    uint32_t ack;
    uint8_t len;
    uint8_t flags;
    uint16_t rwnd;
    uint16_t crc;
    uint16_t urgent;
};
#define PICO_SIZE_TCPHDR (uint32_t)(sizeof(struct pico_tcp_hdr))
#define PICO_TCP_OPTION_END         0x00
#define PICO_TCPOPTLEN_END        1u
#define PICO_TCP_OPTION_NOOP        0x01
#define PICO_TCPOPTLEN_NOOP       1
#define PICO_TCP_OPTION_MSS         0x02
#define PICO_TCPOPTLEN_MSS        4
#define PICO_TCP_OPTION_WS          0x03
#define PICO_TCPOPTLEN_WS         3u
#define PICO_TCP_OPTION_SACK_OK        0x04
#define PICO_TCPOPTLEN_SACK_OK       2
#define PICO_TCP_OPTION_SACK        0x05
#define PICO_TCPOPTLEN_SACK       2 /* Plus the block */
#define PICO_TCP_OPTION_TIMESTAMP   0x08
#define PICO_TCPOPTLEN_TIMESTAMP  10u
enum wolfSSL_ErrorCodes {
    INPUT_CASE_ERROR             = -301,   /* process input state error */
    PREFIX_ERROR                 = -302,   /* bad index to key rounds  */
    MEMORY_ERROR                 = -303,   /* out of memory            */
    VERIFY_FINISHED_ERROR        = -304,   /* verify problem on finished */
    VERIFY_MAC_ERROR             = -305,   /* verify mac problem       */
    PARSE_ERROR                  = -306,   /* parse error on header    */
    UNKNOWN_HANDSHAKE_TYPE       = -307,   /* weird handshake type     */
    SOCKET_ERROR_E               = -308,   /* error state on socket    */
    SOCKET_NODATA                = -309,   /* expected data, not there */
    INCOMPLETE_DATA              = -310,   /* don't have enough data to
                                              complete task            */
    UNKNOWN_RECORD_TYPE          = -311,   /* unknown type in record hdr */
    DECRYPT_ERROR                = -312,   /* error during decryption  */
    FATAL_ERROR                  = -313,   /* recvd alert fatal error  */
    ENCRYPT_ERROR                = -314,   /* error during encryption  */
    FREAD_ERROR                  = -315,   /* fread problem            */
    NO_PEER_KEY                  = -316,   /* need peer's key          */
    NO_PRIVATE_KEY               = -317,   /* need the private key     */
    RSA_PRIVATE_ERROR            = -318,   /* error during rsa priv op */
    NO_DH_PARAMS                 = -319,   /* server missing DH params */
    BUILD_MSG_ERROR              = -320,   /* build message failure    */
    BAD_HELLO                    = -321,   /* client hello malformed   */
    DOMAIN_NAME_MISMATCH         = -322,   /* peer subject name mismatch */
    WANT_READ                    = -323,   /* want read, call again    */
    NOT_READY_ERROR              = -324,   /* handshake layer not ready */
    IPADDR_MISMATCH              = -325,   /* peer ip address mismatch */
    VERSION_ERROR                = -326,   /* record layer version error */
    WANT_WRITE                   = -327,   /* want write, call again   */
    BUFFER_ERROR                 = -328};
typedef uint16_t   word16;
typedef unsigned int   word32;	
typedef unsigned char  byte;		
typedef struct {
//    ALIGN16 byte staticBuffer[STATIC_BUFFER_LEN];
    byte*  buffer;       /* place holder for static or dynamic buffer */
    word32 length;       /* total buffer length used */
    word32 idx;          /* idx to part of length already consumed */
    word32 bufferSize;   /* current buffer size */
    byte   dynamicFlag;  /* dynamic memory currently in use */
    byte   offset;       /* alignment offset attempt */
} bufferStatic;
typedef struct Buffers {
bufferStatic    inputBuffer;
} Buffers;
struct Options {
	word16            tls1_3:1;
};
typedef struct Options Options;
typedef struct CipherSpecs {
    word16 key_size;
    word16 iv_size;
    word16 block_size;
    word16 aead_mac_size;
    byte bulk_cipher_algorithm;
    byte cipher_type;               /* block, stream, or aead */
    byte mac_algorithm;
    byte kea;                       /* key exchange algo */
    byte sig_algo;
    byte hash_size;
    byte pad_size;
    byte static_ecdh;
} CipherSpecs;
typedef struct RecordLayerHeader {
    byte            type;
    byte            pvMajor;
    byte            pvMinor;
    byte            length[2];
} RecordLayerHeader;
typedef struct Keys{
word32 padSz;  
}Keys;
struct WOLFSSL {
  Buffers         buffers;
	Options         options;
	word16          curSize;
	CipherSpecs     specs;
	RecordLayerHeader curRL;
	Keys            keys;
};
		
typedef struct WOLFSSL          WOLFSSL;
