Silbot3 HTML Display ROS node
=============================

Display html files and connect with ros packages.

* Webkit based html rendering.
* Receive ros message in javascript
* Publish ros message in javascript

## Requirement

* Install required packages if not exists.

```sh
sudo apt install python-pyside.qtwebkit python-pyside.qtscript
```

## How to use

### Send url to display

* Send url message to **/silbot3_display/page/show** with **std_msgs/String** type
* Example
  * ```file:///path/to/display/file.html``` for local html file
  * ```http://url.to/display``` for web page

#### Unicode support

* For unicode support, insert codes in html file's ```<head>``` tag  

```html
<head>
    <meta http-equiv="content-type" content="text/html; charset=UTF-8">
    ...
</head>
```

### Send messages in other ros node

* Send message to **/silbot3_display/page/command** with **silbot3_msgs/DisplayMessage** type

### Handle ros messages in javascript

* Receive and handle message with **silbot3_msgs/DisplayMessage** type

```javascript
function handle_ros_command(name, params) {
  // name: String
  // params: String[]
}
```

### Publish ros messages in javascript

```javascript
ROSEvent.publish(name, params)
```

* It will publish message with **/silbot3_display/page/event** and it's message type is **silbot3_msgs/DisplayMessage**

