/**********************************************************************
 *	CODEPAGES.H
 * 	Header file with codepage definitions for dbf
 *  using ISO-8559-1 definitions (Latin 1) to encode
 *  Author: Bjoern Berg, September 2002
 *  Email: clergyman@gmx.de
 *  dbf Reader and converter for dBase
 *  Version 0.2
 *
 *  History:
 *  - Version 0.2 - 2003-01-30
 *	  included patch by Christian Vogel:
 *	  changes all occurences of "char" to "unsigned char"
 *    This avoids many warnings about "case statement out of range"	
 *  - Version 0.1 - 14.09.2002
 *	  first implementation, using iso-definitions
 ********************************************************************/
	
/* * * * CP850CONVERT */
void cp850convert (unsigned char *src)
{
	int unsigned i;
	for(i=0; i < strlen(src); i++) {
		switch(*(src+i)) {
		case 0xE4:
			*(src+i) = (unsigned char)228;	/*auml (GER)*/
			break;
		case 0xC4:
			*(src+i) = (unsigned char)196;	/*Auml (GER)*/
			break;			
		case 0xF6:
			*(src+i) = (unsigned char)246;	/*ouml (GER)*/
			break;
		case 0xD6:
			*(src+i) = (unsigned char)214;	/*Ouml (GER)*/
			break;			
		case 0xFC:
			*(src+i) = (unsigned char)252;	/*uuml (GER)*/
			break;	 
		case 0xDC:
			*(src+i) = (unsigned char)220;	/*Uuml (GER)*/
			break;	
		case 0xDF:
			*(src+i) = (unsigned char)223;	/*szlig (GER)*/
			break;	
		case 0xE9:
			*(src+i) = (unsigned char)233;	/*eacute (FR)*/
			break;
		case 0xE2:
			*(src+i) = (unsigned char)226;	/*acirc (FR)*/
			break;			
		case 0xE0:
			*(src+i) = (unsigned char)233;	/*agrave (FR)*/
			break;	
		case 0xEA:
			*(src+i) = (unsigned char)234;	/*ecirc (FR)*/
			break;	
		case 0xE8:
			*(src+i) = (unsigned char)232;	/*egrave (FR)*/
			break;	
		case 0xEE:
			*(src+i) = (unsigned char)238;	/*icirc	(FR)*/
			break;	
		case 0xEC:
			*(src+i) = (unsigned char)236;	/*igrave (FR)*/
			break;	
		case 0xC9:
			*(src+i) = (unsigned char)201;	/*Eacute (FR)*/
			break;	
		case 0xF4:
			*(src+i) = (unsigned char)244; /*ocirc (FR)*/
			break;	
		case 0xF2:
			*(src+i) = (unsigned char)242;	/*ograve (FR)*/
			break;	
		case 0xFB:
			*(src+i) = (unsigned char)251;	/*ucirc (FR)*/
			break;	
		case 0xF9:
			*(src+i) = (unsigned char)249;	/*ugrave (FR)*/
			break;	
		case 0xE1:
			*(src+i) = (unsigned char)225;	/*aacute (FR)*/
			break;	
		case 0xED:
			*(src+i) = (unsigned char)237;	/*iacute (FR)*/
			break;	
		case 0xF3:
			*(src+i) = (unsigned char)243;	/*oacute (FR)*/
			break;	
		case 0xFA:
			*(src+i) = (unsigned char)250;	/*uacute (FR)*/
			break;	
		case 0xC1:
			*(src+i) = (unsigned char)193;	/*Aacute (FR)*/
			break;		
		case 0xC2:
			*(src+i) = (unsigned char)194;	/*Acirc (FR)*/
			break;		
		case 0xc0:
			*(src+i) = (unsigned char)192;	/*Agrave (FR)*/
			break;		
		case 0xCA:
			*(src+i) = (unsigned char)202;	/*Ecirc (FR)*/
			break;		
		case 0xC8:
			*(src+i) = (unsigned char)200;	/*Egrave (FR)*/
			break;		
		case 0xCD:
			*(src+i) = (unsigned char)205;	/*Iacute (FR)*/
			break;		
		case 0xCE:
			*(src+i) = (unsigned char)206;	/*Icirc (FR)*/
			break;		
		case 0xCC:
			*(src+i) = (unsigned char)204;	/*Igrave (FR)*/
			break;	
		case 0xD3:
			*(src+i) = (unsigned char)211;	/*Oacute (FR)*/
			break;			
		case 0xD4:
			*(src+i) = (unsigned char)212;	/*Ocirc (FR)*/
			break;		
		case 0xD2:
			*(src+i) = (unsigned char)210;	/*Ograve (FR)*/
			break;		
		case 0xDA:
			*(src+i) = (unsigned char)218;	/*Uacute (FR)*/
			break;		
		case 0xDB:
			*(src+i) = (unsigned char)219;	/*Ucirc (FR)*/
			break;		
		case 0xD9:
			*(src+i) = (unsigned char)217;	/*Ugrave (FR)*/
			break;		
		}	
	}
}

/* * * * ASCIInumbers */
void ASCIInumbers (unsigned char *src)
{
	int unsigned i;
	for(i=0; i < strlen(src); i++) {
		switch(*(src+i)) {
		case 0x84:
			*(src+i) = (unsigned char)228;	/*auml (GER)*/
			break;
		case 0x8E:
			*(src+i) = (unsigned char)196;	/*Auml (GER)*/
			break;			
		case 0x94:
			*(src+i) = (unsigned char)246;	/*ouml (GER)*/
			break;
		case 0x99:
			*(src+i) = (unsigned char)214;	/*Ouml (GER)*/
			break;			
		case 0x81:
			*(src+i) = (unsigned char)252;	/*uuml (GER)*/
			break;	 
		case 0x9A:
			*(src+i) = (unsigned char)220;	/*Uuml (GER)*/
			break;	
		case 0xE1:
			*(src+i) = (unsigned char)223;	/*szlig (GER)*/
			break;	
		case 0x82:
			*(src+i) = (unsigned char)233;	/*eacute (FR)*/
			break;
		case 0x83:
			*(src+i) = (unsigned char)226;	/*acirc (FR)*/
			break;			
		case 0x85:
			*(src+i) = (unsigned char)233;	/*agrave (FR)*/
			break;	
		case 0x88:
			*(src+i) = (unsigned char)234;	/*ecirc (FR)*/
			break;	
		case 0x8A:
			*(src+i) = (unsigned char)232;	/*egrave (FR)*/
			break;	
		case 0x8C:
			*(src+i) = (unsigned char)238;	/*icirc	(FR)*/
			break;	
		case 0x8D:
			*(src+i) = (unsigned char)236;	/*igrave (FR)*/
			break;	
		case 0x90:
			*(src+i) = (unsigned char)201;	/*Eacute (FR)*/
			break;	
		case 0x93:
			*(src+i) = (unsigned char)244; /*ocirc (FR)*/
			break;	
		case 0x95:
			*(src+i) = (unsigned char)242;	/*ograve (FR)*/
			break;	
		case 0x96:
			*(src+i) = (unsigned char)251;	/*ucirc (FR)*/
			break;	
		case 0x97:
			*(src+i) = (unsigned char)249;	/*ugrave (FR)*/
			break;	
		case 0xA0:
			*(src+i) = (unsigned char)225;	/*aacute (FR)*/
			break;	
		case 0xA1:
			*(src+i) = (unsigned char)237;	/*iacute (FR)*/
			break;	
		case 0xA2:
			*(src+i) = (unsigned char)243;	/*oacute (FR)*/
			break;	
		case 0xA3:
			*(src+i) = (unsigned char)250;	/*uacute (FR)*/
			break;	
		case 0xB5:
			*(src+i) = (unsigned char)193;	/*Aacute (FR)*/
			break;		
		case 0xB6:
			*(src+i) = (unsigned char)194;	/*Acirc (FR)*/
			break;		
		case 0xB7:
			*(src+i) = (unsigned char)192;	/*Agrave (FR)*/
			break;		
		case 0xD2:
			*(src+i) = (unsigned char)202;	/*Ecirc (FR)*/
			break;		
		case 0xD3:
			*(src+i) = (unsigned char)200;	/*Egrave (FR)*/
			break;		
		case 0xD6:
			*(src+i) = (unsigned char)205;	/*Iacute (FR)*/
			break;		
		case 0xD7:
			*(src+i) = (unsigned char)206;	/*Icirc (FR)*/
			break;		
		case 0xDE:
			*(src+i) = (unsigned char)204;	/*Igrave (FR)*/
			break;	
		case 0xE0:
			*(src+i) = (unsigned char)211;	/*Oacute (FR)*/
			break;			
		case 0xE2:
			*(src+i) = (unsigned char)212;	/*Ocirc (FR)*/
			break;		
		case 0xE3:
			*(src+i) = (unsigned char)210;	/*Ograve (FR)*/
			break;		
		case 0xE9:
			*(src+i) = (unsigned char)218;	/*Uacute (FR)*/
			break;		
		case 0xEA:
			*(src+i) = (unsigned char)219;	/*Ucirc (FR)*/
			break;		
		case 0xEB:
			*(src+i) = (unsigned char)217;	/*Ugrave (FR)*/
			break;				
		}	
	}
}
