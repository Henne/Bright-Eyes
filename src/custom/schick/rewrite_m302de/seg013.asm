.186
.model large

; define a macro for generating the request sections
GEN_REQ	MACRO dummy1, function, redbook, dummy4, dummy6
		db dummy1
		db 0
		db function
		dw 0
		db 8 dup (0)
		db redbook
		dq 0
		db dummy4
		db 0
		db dummy6
		db 7 dup (0)
	ENDM

	public _req
	public _cd_buf1
	public _cd_buf2

.FARDATA

;genterate the request sections
_req db 16h, 0, 85h, 0h, 0h, 0h,0h,0h,0h,0h,0h,0h,0h, 0h, 0h,0h,0h,0h, 0h, 0h,0h, 0h,0h,0h,0h,0h,0h,0h
req1 db 16h, 0, 85h, 0h, 0h, 0h,0h,0h,0h,0h,0h,0h,0h, 1h, 0h,0h,0h,0h, 0h, 0h,0h, 0h,0h,0h,0h,0h,0h,0h
req2 db 1ah, 0, 03h, 0h, 0h, 0h,0h,0h,0h,0h,0h,0h,0h, 0h, 0h,0h,0h,0h, 7h, 0h,0h, 0h,0h,0h,0h,0h,0h,0h
req3 db 1ah, 0, 03h, 0h, 0h, 0h,0h,0h,0h,0h,0h,0h,0h, 0h, 0h,0h,0h,0h, 0bh, 0h,0h, 0h,0h,0h,0h,0h,0h,0h
req4 db 18h, 0, 83h, 0h, 0h, 0h,0h,0h,0h,0h,0h,0h,0h, 0h, 0h,0h,0h,0h, 0h, 0h,2h, 0h,0h,0h,0h,0h,0h,0h
req5 db 16h, 0, 84h, 0h, 0h, 0h,0h,0h,0h,0h,0h,0h,0h, 1h, 0h,0h,0h,0h, 0h, 0h,0h, 0h,0h,0h,0h,0h,0h,0h
req6 db 16h, 0, 85h, 0h, 0h, 0h,0h,0h,0h,0h,0h,0h,0h, 1h, 0h,0h,0h,0h, 0h, 0h,0h, 0h,0h,0h,0h,0h,0h,0h
req7 db 16h, 0, 88h, 0h, 0h, 0h,0h,0h,0h,0h,0h,0h,0h, 1h, 0h,0h,0h,0h, 0h, 0h,0h, 0h,0h,0h,0h,0h,0h,0h
req8 db 16h, 0, 0ch, 0h, 0h, 0h,0h,0h,0h,0h,0h,0h,0h, 0h, 0h,0h,0h,0h, 1h, 0h,0h, 0h,0h,0h,0h,0h,0h,0h
;;GEN_REQ 16h, 85h, 0h, 00h, 0h		;;STOP CDA HSG-mode
;;GEN_REQ 16h, 85h, 1h, 00h, 0h		;;STOP CDA Redbook-mode
;;GEN_REQ 1ah, 03h, 0h, 07h, 0h		;;IOCTL input
;;GEN_REQ 1ah, 03h, 0h, 0bh, 0h		;;IOCTL input
;;GEN_REQ 18h, 83h, 0h, 00h, 2h		;;SEEK (not used)
;;GEN_REQ 16h, 84h, 1h, 00h, 0h		;;PLAY CDA Redbook
;;GEN_REQ 16h, 85h, 1h, 00h, 0h		;;STOP CDA Redbook
;;GEN_REQ 16h, 88h, 1h, 00h, 0h		;;RESUME CDA Redbook
;;GEN_REQ 16h, 0ch, 0h, 01h, 0h		;;?_w


_cd_buf1 db 804 dup (0)			;another buffer
_cd_buf2 db 20 dup (0)			;buffer for IOCTL

end
