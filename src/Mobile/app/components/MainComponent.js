import React, { Component } from 'react';
import {
    ScrollView,
    Text,
    TextInput,
    View,
    Button,
    ActivityIndicator,
    StyleSheet,
    ImageBackground,
    TouchableOpacity
} from 'react-native';

import * as settings from '../config/Settings';
import Footer from './sub-components/Footer';

export default class MainComponent extends Component {
    state = {
        username: ''
    }

    render() {
        aa = 0;
        return (
            <View style={{ flex: 1, marginTop: 20 }}>
                <ScrollView>
                    <Text> One od disd am gdljgnk fadnfk jnfkgjsnskdjndkjds vksdj dsfkdsffdddddddddddddf df dsfds f</Text>
                </ScrollView>
                <View style={styles.container}>
                    <Footer buttons={[{image: require('../content/event.png'), onPress: () => {}},
                                        {image: require('../content/friends.png'), onPress: () => {}}]}></Footer>
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
    }
  });