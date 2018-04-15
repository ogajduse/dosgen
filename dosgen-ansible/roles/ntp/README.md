NTP
=========

Set up Fedora 14 as NTP server with exposed [CVE-2013-5211](http://cve.mitre.org/cgi-bin/cvename.cgi?name=CVE-2013-5211) vulnerability.

Requirements
------------

* Fedora 14 Server after clean installation

Role Variables
--------------

>A description of the settable variables for this role should go here, including any variables that are in defaults/main.yml, vars/main.yml, and any variables that can/should be set via parameters to the role. Any variables that are read from other roles and/or the global scope (ie. hostvars, group vars, etc.) should be mentioned here as well.

Dependencies
------------
No dependencies.
>A list of other roles hosted on Galaxy should go here, plus any details in regards to parameters that may need to be set for other roles, or variables that are used from other roles.

Example Playbook
----------------

Including an example of how to use your role (for instance, with variables passed in as parameters) is always nice for users too:


```yaml
- name: apply the configuration to the ntp server
  hosts: ntp
  remote_user: root

  roles:
  - ntp
```

License
-------

BSD

Author Information
------------------

Ondřej Gajdušek - [email](mailto:gajdusek.giga@seznam.cz)
