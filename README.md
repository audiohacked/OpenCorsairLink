# OpenCorsairLink [![Build Status](https://travis-ci.com/audiohacked/OpenCorsairLink.svg?branch=testing)](https://travis-ci.com/audiohacked/OpenCorsairLink)

OpenCorsairLink is a status & control utility that interacts with CorsairLink products.

### Disclaimer
> __DISCLAIMER__: OpenCorsairLink is not an official Corsair product. It is licensed under the GNU General Public License (version 2) in the hope that it will be useful, but with NO WARRANTY of any kind. Corsair and Corsair Link are trademarks or registered trademarks of Corsair Components, Inc. The trademark holders are not affiliated with the maker of this product and do not endorse this product.

## Installation
### Requirements
* libusb-1.0  
* pkg-config  

```
$ make
$ sudo make install
```

## Usage
Since the program needs direct hardware access you should run this with 'sudo' or as root.  

```
$ sudo ./OpenCorsairLink.elf
$ sudo ./OpenCorsairLink.elf --help
$ sudo ./OpenCorsairLink.elf --device 0
$ sudo ./OpenCorsairLink.elf --device 0 --led channel=0,mode=0,color=00FF00
$ sudo ./OpenCorsairLink.elf --device 0 --fan channel=0,mode=5
$ sudo ./OpenCorsairLink.elf --device 0 --pump mode=5
```

## Development
### Branches
 - master: Stable or Release Version  
 - testing: Testing or Release Canidate  
 - feature\/\*: Cutting Edge Features (Use at your own risk)  

## Contributing
Pull requests are welcome. For major changes, please open an issue first to discuss what you would like to change.

Please make sure to update tests as appropriate.

## Contact

Maintainers reserve the rights to modify and remove issues, pull requests and comments therein, that are denunciating, off-topic, harmful, hateful and overall inappropriate.
Please be appreciative, humble and kind to each other.

* [GitHub Issues](https://github.com/audiohacked/OpenCorsairLink/issues)

## License
[GPLv2](https://choosealicense.com/licenses/gpl-2.0/)
