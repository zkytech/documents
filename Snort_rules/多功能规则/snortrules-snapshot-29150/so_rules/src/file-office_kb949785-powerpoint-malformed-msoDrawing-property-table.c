/*
Does not use built-in detection function!

alert tcp $EXTERNAL_NET $HTTP_PORTS -> $HOME_NET any (msg:"WEB-CLIENT Powerpoint Viewer malformed msoDrawing property table vulnerability"; flow:established, to_client; flowbits:isset,file.ppt; content:"|0B F0|"; metadata:policy balanced-ips drop, policy security-ips drop; reference:cve,2008-0121; classtype:attempted-user; reference:url,technet.microsoft.com/en-us/security/bulletin/ms08-051; sid:13969;)
*/
/*
 * Use at your own risk.
 *
 * Copyright (C) 2005-2008 Sourcefire, Inc.
 * 
 * This file is autogenerated via rules2c, by Brian Caswell <bmc@sourcefire.com>
 */


#include "sf_snort_plugin_api.h"
#include "sf_snort_packet.h"

//#define DEBUG
#ifdef DEBUG
#define DEBUG_WRAP(code) code
#else
#define DEBUG_WRAP(code)
#endif

/* declare detection functions */
int rule13969eval(void *p);

/* declare rule data structures */
/* precompile the stuff that needs pre-compiled */
/* flow:established, to_client; */
static FlowFlags rule13969flow0 = 
{
    FLOW_ESTABLISHED|FLOW_TO_CLIENT
};

static RuleOption rule13969option0 =
{
    OPTION_TYPE_FLOWFLAGS,
    {
        &rule13969flow0
    }
};
/* flowbits:isset "file.ppt"; */
static FlowBitsInfo rule13969flowbits1 =
{
    "file.ppt",
    FLOWBIT_ISSET,
    0,
};

static RuleOption rule13969option1 =
{
    OPTION_TYPE_FLOWBIT,
    {
        &rule13969flowbits1
    }
};
// content:"|0B F0|"; 
static ContentInfo rule13969content2 = 
{
    (uint8_t *) "|0B F0|", /* pattern (now in snort content format) */
    0, /* depth */
    0, /* offset */
    CONTENT_BUF_NORMALIZED|CONTENT_RELATIVE, /* flags */ // XXX - need to add CONTENT_FAST_PATTERN support
    NULL, /* holder for boyer/moore PTR */
    NULL, /* more holder info - byteform */
    0, /* byteform length */
    0 /* increment length*/
};

static RuleOption rule13969option2 = 
{
    OPTION_TYPE_CONTENT,
    {
        &rule13969content2
    }
};

/* references for sid 13969 */
/* reference: cve "2008-0121"; */
static RuleReference rule13969ref1 = 
{
    "cve", /* type */
    "2008-0121" /* value */
};

/* reference: url "technet.microsoft.com/en-us/security/bulletin/ms08-XXX"; */
static RuleReference rule13969ref2 = 
{
    "url", /* type */
    "technet.microsoft.com/en-us/security/bulletin/ms08-051" /* value */
};

static RuleReference *rule13969refs[] =
{
    &rule13969ref1,
    &rule13969ref2,
    NULL
};
/* metadata for sid 13969 */
/* metadata:policy balanced-ips drop, policy security-ips drop; */
static RuleMetaData rule13969service1 =
{
    "service http"
};

static RuleMetaData rule13969policy1 =
{
   "policy max-detect-ips drop"
};

static RuleMetaData *rule13969metadata[] =
{
    &rule13969service1,
    &rule13969policy1,
    NULL
};
RuleOption *rule13969options[] =
{
    &rule13969option0,
    &rule13969option1,
    &rule13969option2,
    NULL
};

Rule rule13969 = {
   
   /* rule header, akin to => tcp any any -> any any               */{
       IPPROTO_TCP, /* proto */
       "$EXTERNAL_NET", /* SRCIP     */
       "$HTTP_PORTS", /* SRCPORT   */
       0, /* DIRECTION */
       "$HOME_NET", /* DSTIP     */
       "any", /* DSTPORT   */
   },
   /* metadata */
   { 
       3,  /* genid (HARDCODED!!!) */
       13969, /* sigid */
       13, /* revision */
   
       "attempted-user", /* classification */
       0,  /* hardcoded priority XXX NOT PROVIDED BY GRAMMAR YET! */
       "FILE-OFFICE Powerpoint Viewer malformed msoDrawing property table buffer overflow attempt",     /* message */
       rule13969refs /* ptr to references */
       ,rule13969metadata
   },
   rule13969options, /* ptr to rule options */
   &rule13969eval, /* use the built in detection function */
   0 /* am I initialized yet? */
};


/* detection functions */
int rule13969eval(void *p) {
    const uint8_t *cursor_normal = 0;
    SFSnortPacket *sp = (SFSnortPacket *) p;
    const uint8_t *end_of_payload, *beg_of_payload;
    uint8_t value=0, rec_instance;
    uint16_t word=0, pid, fcomplex;
    uint32_t op, rec_len;
    int i;

    if(sp == NULL)
        return RULE_NOMATCH;

    if(sp->payload == NULL)
        return RULE_NOMATCH;
    
    // flow:established, to_client;
    if (checkFlow(p, rule13969options[0]->option_u.flowFlags) > 0 ) {
        // flowbits:isset "file.ppt";
        if (processFlowbits(p, rule13969options[1]->option_u.flowBit) > 0) {
            // content:"|0B F0|";
            if(getBuffer(sp, CONTENT_BUF_NORMALIZED, &beg_of_payload, &end_of_payload) <= 0)
               return RULE_NOMATCH;

            cursor_normal = beg_of_payload;

            while (contentMatch(p, rule13969options[2]->option_u.content, &cursor_normal) > 0) {
                if((cursor_normal + 4 >= end_of_payload) || (cursor_normal - 2 < beg_of_payload))
                   return RULE_NOMATCH;
        
                rec_len = *cursor_normal++;                        
                rec_len |= (*cursor_normal++) << 8;
                rec_len |= (*cursor_normal++) << 16;
                rec_len |= (*cursor_normal++) << 24;

                if(   (cursor_normal + rec_len - 4 >= end_of_payload) ||
                      (cursor_normal + rec_len < cursor_normal))
                   return RULE_NOMATCH;

                // MSO Drawing Property Table Property Table Record Instance
                // cursor_normal = cursor_normal -8;
                value = *(cursor_normal-8);                        
        
  DEBUG_WRAP(printf("WORD: %d\n", value));

                 rec_instance=(value>>4) & 0x0f;

  DEBUG_WRAP(printf("RECORD INSTANCE: %d\n", rec_instance));                  
                    
                 // Loop through all FOPTE objects
                 for(i=0; i < rec_instance; i++) {
                     if(cursor_normal + 6 >= end_of_payload) 
                        return RULE_NOMATCH;

                     word = *cursor_normal++;                        
                     word |= *cursor_normal++ << 8;
                     pid = word & 0x3fff;
                     fcomplex = word & 0x8000;

                     op = *cursor_normal++;                        
                     op |= *cursor_normal++ << 8;
                     op |= *cursor_normal++ << 16;
                     op |= *cursor_normal++ << 24;
                  
  DEBUG_WRAP(printf("PID: %d\n", pid));
  DEBUG_WRAP(printf("FCOMPLEX: %d\n", fcomplex));                  

                     if ((fcomplex || pid==272) && (op >= 0xFFFFFFFC)) { 
                         return RULE_MATCH;
                     }
                }
            }
        }
    }
    return RULE_NOMATCH;
}

/*
Rule *rules[] = {
    &rule13969,
    NULL
};
*/
