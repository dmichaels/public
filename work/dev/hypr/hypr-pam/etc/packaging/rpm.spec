# Copyright (c) 2021, HYPR Corporation
# All rights reserved.
#
# Redistribution and use in source and binary forms, with or without
# modification, are permitted provided that the following conditions are met:
#
#  - Redistributions of source code must retain the above copyright
#    notice, this list of conditions and the following disclaimer.
#  - Redistributions in binary form must reproduce the above copyright
#    notice, this list of conditions and the following disclaimer in the
#    documentation and/or other materials provided with the distribution.
#
# THIS SOFTWARE IS PROVIDED BY THE AUTHOR AND CONTRIBUTORS "AS IS" AND ANY
# EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
# WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
# DISCLAIMED. IN NO EVENT SHALL THE AUTHOR OR CONTRIBUTORS BE LIABLE FOR ANY
# DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
# (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
# LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
# ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
# (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF
# THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
# ------------------------------------------------------------------------------

Name: hypr-pam
#
# Version: 1.0
# Get the version from the top-level directory VERSION file.
#
Version: %(cat %{_topdir}/../../../VERSION | cut -d. -f1-2)
Release: 1
Summary: HYPR Linux PAM for authentication
License: BSD
Group: Applications/System
Provides: hypr-pam
Source: hypr-pam-%{version}.tar.gz
URL: https://g.hypr.com/david.michaels/pam_hypr
Prefix: %{_prefix}
%description
The hypr-pam package provides a Linux PAM supporting HYPR authentication for password-less auth  entication
%prep -q
echo "FYI: _rpmdir = %{_rpmdir}"
echo "FYI: _rpmfilename = %{_rpmfilename}"
echo "FYI: _sourcedir = %{_sourcedir}"
echo "FYI: _buildrootdir = %{_buildrootdir}"
echo "FYI: _buildroot = %{_buildroot}"
echo "FYI: _topdir = %{_topdir}"
echo "FYI: _top_srcdir = %{_top_srcdir}"
echo "FYI: _prefix = %{_prefix}"
echo "FYI: _exec_prefix = %{_exec_prefix}"
echo "FYI: _bindir = %{_bindir}"
echo "FYI: _sbindir = %{_sbindir}"
echo "FYI: _libexecdir = %{_libexecdir}"
echo "FYI: _datadir = %{_datadir}"
echo "FYI: _sysconfdir = %{_sysconfdir}"
echo "FYI: _sharedstatedir = %{_sharedstatedir}"
echo "FYI: _localstatedir = %{_localstatedir}"
echo "FYI: _lib = %{_lib}"
echo "FYI: _libdir = %{_libdir}"
echo "FYI: _libsecuritydir = %{_libsecuritydir}"
echo "FYI: _includedir = %{_includedir}"
echo "FYI: _infodir = %{_infodir}"
echo "FYI: _mandir = %{_mandir}"
echo "FYI: _build = %{_build}"
echo "FYI: _build_alias = %{_build_alias}"
echo "FYI: _build_os = %{_build_os}"
echo "FYI: _build_vendor = %{_build_vendor}"
echo "FYI: _build_arch = %{_build_arch}"
echo "FYI: _build_cpu = %{_build_cpu}"
echo "FYI: _alias = %{_alias}"
echo "FYI: _os = %{_os}"
echo "FYI: _vendor = %{_vendor}"
echo "FYI: _arch = %{_arch}"
echo "FYI: _cpu = %{_cpu}"
echo "FYI: _host = %{_host}"
echo "FYI: _host_alias = %{_host_alias}"
echo "FYI: _host_cpu = %{_host_cpu}"
echo "FYI: _host_vendor = %{_host_vendor}"
echo "FYI: _host_os = %{_host_os}"
echo "FYI: _target = %{_target}"
echo "FYI: _target_alias = %{_target_alias}"
echo "FYI: _target_os = %{_target_os}"
echo "FYI: _target_vendor = %{_target_vendor}"
echo "FYI: _target_arch = %{_target_arch}"
echo "FYI: _target_cpu = %{_target_cpu}"
%setup -q
mkdir m4
%build
autoreconf -iv
%configure --with-libsecuritydir=%{_libsecuritydir} --sysconfdir=/etc HYPR_HTTP_STATIC_LIBCURL=%{HYPR_HTTP_STATIC_LIBCURL}
make PREFIX=%{_prefix}
%install
rm -rf $RPM_BUILD_ROOT
make install prefix=%{_prefix} INSTALL="%{__install} -p" DESTDIR=$RPM_BUILD_ROOT
%post
if [ -f /etc/os-release ] ; then
    OS=`grep '^ID=' /etc/os-release | sed 's/ID=//' | sed 's/"//g'`
    OS_VERSION=`grep '^VERSION_ID=' /etc/os-release | sed 's/VERSION_ID=//' | sed 's/"//g'`
    case $OS in
       *opensuse* | *sles* )
           if [ -f /usr/lib64/security/pam_hypr.so ] ; then
               echo "Post installation step for openSUSE ..."
               echo "Moving /usr/lib64/security/pam_hypr.so to /lib64/security"
               mv /usr/lib64/security/pam_hypr.so /lib64/security
           fi
           ;;
       *elementary* )
           if [ -f /usr/lib/`arch`-linux-gnu/security/pam_hypr.so -a -d /lib/`arch`-linux-gnu/security ] ; then
               echo "Post installation step for elementary OS ..."
               echo "Moving /usr/lib/`arch`-linux-gnu/security/pam_hypr.so to /lib/`arch`-linux-gnu/security"
               mv /usr/lib/`arch`-linux-gnu/security/pam_hypr.so /lib/`arch`-linux-gnu/security
           fi
           ;;
       *debian* )
           if [ $OS_VERSION -lt 10 ] ; then
               if [ -f /usr/lib/`arch`-linux-gnu/security/pam_hypr.so -a -d /lib/`arch`-linux-gnu/security ] ; then
                   echo "Post installation step for Debian $OS_VERSION ..."
                   echo "Moving /usr/lib/`arch`-linux-gnu/security/pam_hypr.so to /lib/`arch`-linux-gnu/security"
                   mv /usr/lib/`arch`-linux-gnu/security/pam_hypr.so /lib/`arch`-linux-gnu/security
               fi
           fi
           ;;
    esac
fi
%postun
if [ -f /etc/os-release ] ; then
    OS=`grep '^ID=' /etc/os-release | sed 's/ID=//' | sed 's/"//g'`
    OS_VERSION=`grep '^VERSION_ID=' /etc/os-release | sed 's/VERSION_ID=//' | sed 's/"//g'`
    case $OS in
       *opensuse* | *sles* )
           if [ -f /lib64/security/pam_hypr.so ] ; then
               echo "Post uninstallation step for openSUSE ..."
               echo "Removing /lib64/security/pam_hypr.so"
               rm -f /lib64/security/pam_hypr.so
           fi
           ;;
       *elementary* )
           if [ -f /lib/`arch`-linux-gnu/security/pam_hypr.so ] ; then
               echo "Post uninstallation step for elementary OS ..."
               echo "Removing /lib/`arch`-linux-gnu/security/pam_hypr.so"
               rm -f /lib/`arch`-linux-gnu/security/pam_hypr.so
           fi
           ;;
       *debian* )
           if [ $OS_VERSION -lt 10 ] ; then
               if [ -f /lib/`arch`-linux-gnu/security/pam_hypr.so ] ; then
                   echo "Post uninstallation step for Debian $OS_VERSION ..."
                   echo "Removing /lib/`arch`-linux-gnu/security/pam_hypr.so"
                   rm -f /lib/`arch`-linux-gnu/security/pam_hypr.so
               fi
           fi
           ;;
    esac
fi
%clean
rm -rf $RPM_BUILD_ROOT
%files
%defattr(-,root,root)
%{_libsecuritydir}/pam_hypr.so
%{_sysconfdir}/pam.d/hypr-example
%{_sysconfdir}/security/pam_hypr.conf-example
%{_bindir}/hypr
%exclude %{_libsecuritydir}/pam_hypr.la
