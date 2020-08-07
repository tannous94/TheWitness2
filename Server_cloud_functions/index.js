const functions = require('firebase-functions');
const admin = require('firebase-admin');
admin.initializeApp();

// fire store function to deploy: firebase deploy --only functions

/*exports.pins = functions.https.onRequest(async (req, res) => {
  let docRef = admin.firestore().collection('Puzzles').doc('Level1');
  promise = docRef.get();
  p = promise.then(snapshot => {
	data = snapshot.data();
	console.log("data is: ");
	console.log(data);
	pin1 = data["value"];
	pin2 = data["value"];
	let updateData = docRef.update({pin1: req.query.color1, pin2: req.query.color2}); // query enables data in url, num is the parameter.
    return res.send(data);
  })
  .catch(error => {
	console.log(error);
	return res.status(500).send(error);
  })
});*/

exports.getpins = functions.region('europe-west3').https.onRequest(async (req, res) => {
	// what we need to do here is, get to the Last Login document and from there extract the last login, then go to the document of that account and take the check point level from there and put it instead of Level1 below.
  let docRef2 = admin.firestore().collection('LoggedIn').doc('CurrentLogin');//.get('Email');
  snapshot2 = await docRef2.get();
  doc2 = snapshot2.data();
  usr = doc2["Email"];  
  if (usr === "NON") {
		return res.send(usr);
  }
  
  let docRef1 = admin.firestore().collection('Accounts').doc(`${usr}`);//.get('Email');
  snapshot1 = await docRef1.get();
  doc1 = snapshot1.data();
  usr1 = doc1["Level"].toString();  
  lvl = "Level" + usr1;
  //return res.send(usr1);
  
  let docRef = admin.firestore().collection('Puzzles').doc(`${lvl}`);
  promise = docRef.get();
  p = promise.then(snapshot => {
	data = snapshot.data();
	let updateData = docRef.get({pin1: req.query.color1, pin2: req.query.color2, pin3: req.query.color3, pin4: req.query.color4, pin5: req.query.color5, pin6: req.query.color6, pin7: req.query.color7, pin8: req.query.color8, pin9: req.query.color9, pin10: req.query.color10, pin11: req.query.color11, pin12: req.query.color12, pin13: req.query.color13, pin14: req.query.color14, pin15: req.query.color15, pin16: req.query.color16}); 
	// query enables data in url, num is the parameter.
    return res.send(data);
  })
  .catch(error => {
	console.log(error);
	return res.status(500).send(error);
  })
});

exports.acc = functions.region('europe-west3').https.onRequest(async (req, res) => {
	// what we need to do here is, get to the Last Login document and from there extract the last login, then go to the document of that account and take the check point level from there and put it instead of Level1 below.
  let docRef2 = admin.firestore().collection('LoggedIn').doc('CurrentLogin');//.get('Email');
  snapshot2 = await docRef2.get();
  doc2 = snapshot2.data();  
  usr = doc2["Email"]; 
  if (usr === "NON") {
		return res.send(usr);
  }
  
  let docRef1 = admin.firestore().collection('Accounts').doc(`${usr}`);//.get('Email');
  snapshot1 = await docRef1.get();
  doc1 = snapshot1.data();
  lvl = doc1["Level"].toString();
  pts = doc1["Points"].toString();
  rank = doc1["Rank"].toString();
  time = doc1["Time Elapsed"];
  
    return res.send({lvl, pts, rank, time});

});

exports.update_acc = functions.region('europe-west3').https.onRequest(async (req, res) => {
	// what we need to do here is, get to the Last Login document and from there extract the last login, then go to the document of that account and take the check point level from there and put it instead of Level1 below.
  let docRef2 = admin.firestore().collection('LoggedIn').doc('CurrentLogin');//.get('Email');
  snapshot2 = await docRef2.get();
  doc2 = snapshot2.data();  
  usr = doc2["Email"]; 
  if (usr === "NON") {
		return res.send(usr);
  }
  
  let docRef = admin.firestore().collection('Accounts').doc(`${usr}`);
  promise = docRef.get();
  p = promise.then(snapshot => {
	data = snapshot.data();
	let updateData = docRef.update({Level: req.query.lvl, Points: req.query.pts, Rank: req.query.rnk, "Time Elapsed": req.query.tim}); 
	// query enables data in url, num is the parameter.
    return res.send(data);
  })
  .catch(error => {
	console.log(error);
	return res.status(500).send(error);
  })

});

/*exports.acc = functions.https.onRequest(async (req, res) => {
	// what we need to do here is, get to the Last Login document and from there extract the last login, then go to the document of that account and take the check point level from there and put it instead of Level1 below.
  let docRef = admin.firestore().collection('LoggedIn').doc('CurrentLogin');//.get('Email');
  snapshot = await docRef.get();
  doc = snapshot.data();
  usr = doc["Email"];  
  
  let docRef1 = admin.firestore().collection('Accounts').doc(`${usr}`);//.get('Email');
  snapshot1 = await docRef1.get();
  doc1 = snapshot1.data();
  usr1 = doc1["Level"].toString();  
  return res.send(usr1);

 
  //return res.send(usr);

  //console.log('Document data:', doc.data());

  
});*/
