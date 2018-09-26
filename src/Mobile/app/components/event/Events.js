import React, { Component } from 'react';
import {
    Text,
    View,
    StyleSheet,
    TouchableOpacity,
    Image
} from 'react-native';


export default class Events extends Component {
    alertItemName = (item) => {
       alert(item.name)
    }
    render() {
        return (
            <View> 
               {
                  this.props.events.map((item, index) => (
                     <TouchableOpacity key = {item.id} style = {styles.container} 
                            onPress = {() => this.alertItemName(item)}>
                        <Text style = {styles.text}>
                           {item.name}
                        </Text>
                     </TouchableOpacity>
                  ))
               }
            </View>
        )
    }
}


const styles = StyleSheet.create ({
    container: {
       padding: 10,
       marginTop: "1%",
       marginLeft: "2%",
       marginRight: "2%",
       backgroundColor: '#a2b9bc',
       alignItems: 'center',
       borderRadius: 50,
    },
    text: {
       color: '#4f603c'
    }
 })