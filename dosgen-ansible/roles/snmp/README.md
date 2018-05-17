Role Name
=========

Set up Fedora >= 23 as SNMP v2 agent with `public` community string.

Requirements
------------
* Fresh-installed instance of Fedora >=23
* Package `python2-pysnmp` is a dependency on a system which runs the playbook. You can simply install it via `dnf` package manager.

Role Variables
--------------

>A description of the settable variables for this role should go here, including any variables that are in defaults/main.yml, vars/main.yml, and any variables that can/should be set via parameters to the role. Any variables that are read from other roles and/or the global scope (ie. hostvars, group vars, etc.) should be mentioned here as well.

Dependencies
------------
No dependencies.
>A list of other roles hosted on Galaxy should go here, plus any details in regards to parameters that may need to be set for other roles, or variables that are used from other roles.

Example Playbook
----------------

```yaml
- name: apply the configuration to the snmp server
  hosts: ntp
  remote_user: root

  roles:
  - snmp
```

License
-------

BSD

Author Information
------------------

Ondřej Gajdušek - [email](mailto:gajdusek.giga@seznam.cz)
