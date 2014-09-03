COMPILER    = tcc -c -1 -B -S -ml -w -Ic:\tc\include -I..\..\lib\include
HEADFILE    = common.h cut_reg.h keycmd.h order.h language.h ord_edit.h passwd.h yes_no.h reg_core.h order.h config.h  tmp_cfg.h setup.h status.h

dstfile     = cut.lib

Lib         = $(dstfile)
LibList1    =  start.obj  gui.obj  preload.obj  main.obj  status.obj  language.obj  ord_edit.obj
LibFile1    = +start.obj +gui.obj +preload.obj +main.obj +status.obj +language.obj +ord_edit.obj
LibList2    =  order.obj  config.obj  tmp_cfg.obj  keycmd.obj  setup.obj  setup_su.obj  setup_dr.obj
LibFile2    = +order.obj +config.obj +tmp_cfg.obj +keycmd.obj +setup.obj +setup_su.obj +setup_dr.obj
LibList3    =  welcome.obj  passwd.obj  yes_no.obj  reg_core.obj  r_machid.obj  add_sub.obj
LibFile3    = +welcome.obj +passwd.obj +yes_no.obj +reg_core.obj +r_machid.obj +add_sub.obj
LibList4    =  version.obj  cut_reg.obj  show_plt.obj
LibFile4    = +version.obj +cut_reg.obj +show_plt.obj


Libtemp1    = __tmp1.lib
Libtemp2    = __tmp2.lib
Libtemp3    = __tmp3.lib
Libtemp4    = __tmp4.lib

start.obj: start.c $(HEADFILE)
	$(COMPILER) start.c

version.obj: version.c $(HEADFILE)
	$(COMPILER) version.c

cut_reg.obj: cut_reg.c $(HEADFILE)
	$(COMPILER) cut_reg.c

gui.obj: gui.c $(HEADFILE)
	$(COMPILER) gui.c

preload.obj: preload.c $(HEADFILE)
	$(COMPILER) preload.c

main.obj: main.c $(HEADFILE)
	$(COMPILER) main.c

status.obj: status.c $(HEADFILE)
	$(COMPILER) status.c

order.obj: order.c $(HEADFILE)
	$(COMPILER) order.c

config.obj: config.c $(HEADFILE)
	$(COMPILER) config.c

tmp_cfg.obj: tmp_cfg.c $(HEADFILE)
	$(COMPILER) tmp_cfg.c

keycmd.obj: keycmd.c $(HEADFILE)
	$(COMPILER) keycmd.c

setup.obj: setup.c $(HEADFILE)
	$(COMPILER) setup.c

setup_su.obj: setup_su.c $(HEADFILE)
	$(COMPILER) setup_su.c

setup_dr.obj: setup_dr.c $(HEADFILE)
	$(COMPILER) setup_dr.c

language.obj: language.c $(HEADFILE)
	$(COMPILER) language.c

welcome.obj: welcome.c $(HEADFILE)
	$(COMPILER) welcome.c

ord_edit.obj: ord_edit.c $(HEADFILE)
	$(COMPILER) ord_edit.c

show_plt.obj: show_plt.c $(HEADFILE)
	$(COMPILER) show_plt.c

passwd.obj: passwd.c $(HEADFILE)
	$(COMPILER) passwd.c

yes_no.obj: yes_no.c $(HEADFILE)
	$(COMPILER) yes_no.c

add_sub.obj: add_sub.c $(HEADFILE)
	$(COMPILER) add_sub.c

reg_core.obj: reg_core.c $(HEADFILE)
	$(COMPILER) reg_core.c

r_machid.obj: r_machid.c $(HEADFILE)
	$(COMPILER) r_machid.c


$(Libtemp1):  $(LibList1)
	del  $(Libtemp1)
	tlib $(Libtemp1) $(LibFile1)

$(Libtemp2):  $(LibList2)
	del  $(Libtemp2)
	tlib $(Libtemp2) $(LibFile2)

$(Libtemp3):  $(LibList3)
	del  $(Libtemp3)
	tlib $(Libtemp3) $(LibFile3)

$(Libtemp4):  $(LibList4)
	del  $(Libtemp4)
	tlib $(Libtemp4) $(LibFile4)


$(Lib):  $(Libtemp1) $(Libtemp2) $(Libtemp3)  $(Libtemp4)
	del  $(Lib)
	tlib $(Lib) +$(Libtemp1) +$(Libtemp2) +$(Libtemp3) +$(Libtemp4)



