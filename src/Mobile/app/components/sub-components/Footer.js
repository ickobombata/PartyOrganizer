import React, { Component } from 'react';
import {
    Text,
    View,
    StyleSheet,
    TouchableOpacity,
    Image
} from 'react-native';


export default class Footer extends Component {
    render() {
        return (
            <View style={styles.container}>
                {
                    this.props.buttons.slice(0, -1).map(function(x, i) {
                        return <View key={i} style={styles.buttonPaddingRight}>
                                    <TouchableOpacity key={i} style={styles.button} onPress={x.onPress()}>
                                        <Image key={i} source={x.image}/>
                                    </TouchableOpacity>
                                </View>
                    })
                }
                <View style={styles.buttonNoPadding}>
                    <TouchableOpacity style={styles.button} onPress={this.props.buttons[this.props.buttons.length - 1].onPress()}>
                        <Image source={this.props.buttons[this.props.buttons.length - 1].image}/>
                    </TouchableOpacity>
                </View>
            </View>
        )
    }
}

const styles = StyleSheet.create({
    container: {
      flex: 1,
      flexDirection: 'row',
      alignItems: 'flex-end',
      justifyContent: 'space-around'
    },
    buttonPaddingRight: {
      flex: 1,
      paddingRight: 5
    },
    buttonNoPadding: {
        flex: 1
    },
    button: {
      backgroundColor: '#859a9b',
      borderRadius: 30,
      padding: 5,
      marginBottom: 5,
      shadowColor: '#303838',
      shadowOffset: { width: 0, height: 5 },
      shadowRadius: 10,
      shadowOpacity: 0.35,
      alignItems: 'center'
    }
  });