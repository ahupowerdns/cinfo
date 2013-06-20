<html>
<head><title>Cacheinfo project</title></head>
<body bgcolor=#ffffff>
<center>
<script type="text/javascript"><!--
google_ad_client = "pub-9976612598143264";
google_ad_width = 728;
google_ad_height = 90;
google_ad_format = "728x90_as";
google_ad_channel ="";
//--></script>
<script type="text/javascript"
  src="http://pagead2.googlesyndication.com/pagead/show_ads.js">
</script>
</center>
<h1>cinfo</h1>
&copy;2001 by bert hubert &lt;ahu@ds9a.nl&gt;. Licensed under the  
<a href=cvs/COPYING>GNU GPL</a> <p>

The goal of this project is to provide users with more information on the
contents of the various caches of their kernel. Having more information may
be useful to:
	
<ul>
<li>determine if adding more memory is useful</li>
<li>see if the cache is functioning as it should</li>
<li>find out if your files on the server are being cached or if other
	  files are deemed more important</li>
</ul>

It is suspected that better awareness of caches will improve people's
ability to build and maintain high performance servers and kernels.
<p>
<h2>News</H2>
<table border=1>
<tr><td valign=top>2001-06-30</td><td>Initial release. Pretty functional in
telling you about how much of a file is in the pagecache. Example:
<pre>
$ cinfo /lib/libc.so.6
/lib/libc.so.6: 275 pages, 275 pages in core (100.00%)
</pre>
</table>

<a name="download"></a>
<H2>Distribution</H2>
Version 0.1 is available <a
href=http://ds9a.nl/cinfo/cinfo-0.1.tar.gz>here</a> and works on Linux 2.4
and FreeBSD 4.3. It may work on lower versions, if so, please let me know.

<H2>Bazaar</H2>
This project is intended to be very much a <a
href="http://www.tuxedo.org/~esr/writings/cathedral-bazaar/">Bazaar</a> style development. If it

were to be any more open, bits would fall out. 
<p>
A CVS tree is available. try this:
<pre>
$ export CVSROOT=:pserver:anon@outpost.ds9a.nl:/var/cvsroot
$ cvs login
CVS password: [enter 'cvs' (without 's)]
$ cvs co cinfo
cvs server: Updating cinfo
...
</pre>

<p>

<a href=http://www.powerdns.com>
This site made possible by PowerDNS, for all your domain needs.
</a>
<!-- Search Google -->
<center>
<FORM method=GET action="http://www.google.com/search">
<TABLE bgcolor="#FFFFFF"><tr><td>
<A HREF="http://www.google.com/">
<IMG SRC="http://www.google.com/logos/Logo_40wht.gif" border="0"
ALT="Google" align="absmiddle"></A>
<INPUT TYPE=text name=q size=31 maxlength=255 value="">
<INPUT type=submit name=sa VALUE="Google Search">
</td></tr></TABLE>
</FORM>
</center>
<!-- Search Google -->
<center>
<small>
$Id: index.php3,v 1.2 2001/06/30 21:00:08 ahu Exp $
</small>
</center>
</body>
</html>