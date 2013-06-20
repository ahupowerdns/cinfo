<html>
<head><title>Cacheinfo project</title></head>
<body bgcolor=#ffffff>
<h1>cinfo</h1>
&copy;2001 - 2013 by bert hubert &lt;ahu@ds9a.nl&gt;. Licensed under the  
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
<tr><td valign=top>2013-06-20</td><td>Moved to <a href="https://github.com/ahupowerdns/cinfo">GitHub</a>! </td></tr>
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

<H2>Contributing</H2>
<p>
Also, follow us on GitHub: <a href="https://github.com/ahupowerdns/cinfo">https://github.com/ahupowerdns/cinfo</a>
</p>
</body>
</html>