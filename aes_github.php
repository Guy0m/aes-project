<html>
Author : Guillaume Durupt
<center><h3>Implementation of AES 128bits Mode ECB Electronic Code Book</h3></center>

<form name="form" method="post" action="<?=$_SERVER['PHP_SELF']?>"> </br>
<table><tr>
<td>Plaintext message   </td>
<td><input type="text" name="message" value= "<?php if (isset($_POST['message'])) {echo $_POST['message'];} ?>" /></td>
</tr>

<tr>
<td>Plaintext key  </td>
<td><input type="text" name="key" value="<?php if (isset($_POST['key'])) {echo $_POST['key'];} ?>" /></td>
<td><input type="submit" name="send" value="Cipher !"  style="height:21px;width:100px"></td>
</tr></table></form>

<?php // Functions :)
function String2Hex($string){
	$hex='';
	for ($i=0; $i < strlen($string); $i++){
		$hex .= dechex(ord($string[$i]));
	}
	return $hex;
}

function complete($string){ // complete par un zero à gauche
	return str_pad($string,  2, "0",STR_PAD_LEFT);  
}

function display($tableau){ 
	echo '<table border="1">';
	for($r=0;$r<4;$r++){
		echo '<tr>';
		for($c=0;$c<4;$c++){	
			echo '<td align="center">'.complete(dechex($tableau[$r][$c])).'</td>';
		}
	}
	echo '</table>';
}

function SubBytes($word){ 

	$sBox = array(
	array(0x63,0xca,0xb7,0x4,0x09,0x53,0xd0,0x51,0xcd,0x60,0xe0,0xe7,0xba,0x70,0xe1,0x8c),
	array(0x7c,0x82,0xfd,0xc7,0x83,0xd1,0xef,0xa3,0x0c,0x81,0x32,0xc8,0x78,0x3e,0xf8,0xa1),
	array(0x77,0xc9,0x93,0x23,0x2c,0x0,0xaa,0x40,0x13,0x4f,0x3a,0x37,0x25,0xb5,0x98,0x89),
	array(0x7b,0x7d,0x26,0xc3,0x1a,0xed,0xfb,0x8f,0xec,0xdc,0x0a,0x6d,0x2e,0x66,0x11,0x0d),
	array(0xf2,0xfa,0x36,0x18,0x1b,0x20,0x43,0x92,0x5f,0x22,0x49,0x8d,0x1c,0x48,0x69,0xbf),
	array(0x6b,0x59,0x3f,0x96,0x6e,0xfc,0x4d,0x9d,0x97,0x2a,0x6,0xd5,0xa6,0x3,0xd9,0xe6),
	array(0x6f,0x47,0xf7,0x5,0x5a,0xb1,0x33,0x38,0x44,0x90,0x24,0x4e,0xb4,0xf6,0x8e,0x42),
	array(0xc5,0xf0,0xcc,0x9a,0xa0,0x5b,0x85,0xf5,0x17,0x88,0x5c,0xa9,0xc6,0x0e,0x94,0x68),
	array(0x30,0xad,0x34,0x7,0x52,0x6a,0x45,0xbc,0xc4,0x46,0xc2,0x6c,0xe8,0x61,0x9b,0x41),
	array(0x1,0xd4,0xa5,0x12,0x3b,0xcb,0xf9,0xb6,0xa7,0xee,0xd3,0x56,0xdd,0x35,0x1e,0x99),
	array(0x67,0xa2,0xe5,0x80,0xd6,0xbe,0x2,0xda,0x7e,0xb8,0xac,0xf4,0x74,0x57,0x87,0x2d),
	array(0x2b,0xaf,0xf1,0xe2,0xb3,0x39,0x7f,0x21,0x3d,0x14,0x62,0xea,0x1f,0xb9,0xe9,0x0f),
	array(0xfe,0x9c,0x71,0xeb,0x29,0x4a,0x50,0x10,0x64,0xde,0x91,0x65,0x4b,0x86,0xce,0xb0),
	array(0xd7,0xa4,0xd8,0x27,0xe3,0x4c,0x3c,0xff,0x5d,0x5e,0x95,0x7a,0xbd,0xc1,0x55,0x54),
	array(0xab,0x72,0x31,0xb2,0x2f,0x58,0x9f,0xf3,0x19,0x0b,0xe4,0xae,0x8b,0x1d,0x28,0xbb),
	array(0x76,0xc0,0x15,0x75,0x84,0xcf,0xa8,0xd2,0x73,0xdb,0x79,0x8,0x8a,0x9e,0xdf,0x16));

	for ($r=0; $r<4; $r++) {
		for ($c=0; $c<4; $c++){
			$word[$r][$c]= $sBox[($word[$r][$c]&0x0f)][(($word[$r][$c]>>4)&0x0f)];	
		}
	}
	return $word;
}

function shiftRows($word) {	//a optimiser

	$temp=$word;		
	$word[1][3]=$temp[1][0]; // rotation 1bits
	$word[1][2]=$temp[1][3];
	$word[1][1]=$temp[1][2];
	$word[1][0]=$temp[1][1];

	$word[2][3]=$temp[2][1];  // rotation 2bits
	$word[2][2]=$temp[2][0];
	$word[2][1]=$temp[2][3];
	$word[2][0]=$temp[2][2];
	
	$word[3][3]=$temp[3][2];	// rotation 3bits
	$word[3][2]=$temp[3][1];
	$word[3][1]=$temp[3][0];
	$word[3][0]=$temp[3][3];

	return $word;   
}

function mixColumns($word) {  // a finaliser

	for ($c=0; $c<4; $c++) {
		$a = array(4);
		$b = array(4); 
		for ($i=0; $i<4; $i++) {
			$a[$i] = $word[$i][$c];
			$b[$i] = $word[$i][$c]&0x80 ? $word[$i][$c]<<1 ^ 0x011b :$word[$i][$c]<<1;
		}
		$word[0][$c] = $b[0] ^ $a[1] ^ $b[1] ^ $a[2] ^ $a[3];
		$word[1][$c] = $a[0] ^ $b[1] ^ $a[2] ^ $b[2] ^ $a[3];
		$word[2][$c] = $a[0] ^ $a[1] ^ $b[2] ^ $a[3] ^ $b[3]; 
		$word[3][$c] = $a[0] ^ $b[0] ^ $a[1] ^ $a[2] ^ $b[3]; 
	}
	return $word;
}

function keyExpansion($key,$round) {
	$oldkey=$key;
	$rCon = array( 
	array(0x01, 0x00, 0x00, 0x00),
	array(0x02, 0x00, 0x00, 0x00),
	array(0x04, 0x00, 0x00, 0x00),
	array(0x08, 0x00, 0x00, 0x00),
	array(0x10, 0x00, 0x00, 0x00),
	array(0x20, 0x00, 0x00, 0x00),
	array(0x40, 0x00, 0x00, 0x00),
	array(0x80, 0x00, 0x00, 0x00),
	array(0x1b, 0x00, 0x00, 0x00),
	array(0x36, 0x00, 0x00, 0x00),
	array(0x6c, 0x00, 0x00, 0x00),); 
	
	$temp=SubBytes($key); // subytes de la 4eme colonne 
	$temp2=$temp[0][3];	//puis rotword
	$key[0][3]=$temp[1][3];
	$key[1][3]=$temp[2][3];
	$key[2][3]=$temp[3][3];
	$key[3][3]=$temp2;

	for($i=0;$i<4;$i++){  // puis xor avec rcon
		$key[$i][0] = $key[$i][0] ^ $key[$i][3] ^ $rCon[($round-1)][$i];
	}
	
	for($j=0;$j<3;$j++){
		for($i=0;$i<4;$i++){
			$key[$i][($j+1)] = $key[$i][$j] ^  $oldkey[$i][($j+1)];
		}	
	}
	return $key;
}

//END FUNCTIONS

$matrix_msg = array(		// exemple message clair par defaut
array(0x32, 0x88, 0x31, 0xe0),
array(0x43, 0x5a, 0x31, 0x37),
array(0xf6, 0x30, 0x98, 0x07),
array(0xa8, 0x8d, 0xa2, 0x34),);

$cle = array(			// exemple cle clair par defaut
array(0x2b, 0x28, 0xab, 0x09),
array(0x7e, 0xae, 0xf7, 0xcf),
array(0x15, 0xd2, 0x15, 0x4f),
array(0x16, 0xa6, 0x88, 0x3c),);

if (isset($_POST['send'])) {
	
	$message = $_POST['message'];
	$cleachiffre = $_POST['key'];

	$matrix_msg = array(array(0,0,0,0),array(0,0,0,0),array(0,0,0,0),array(0,0,0,0));	
	$cle = array(array(0,0,0,0),array(0,0,0,0),array(0,0,0,0),array(0,0,0,0));

	$i=0;
	$j=0;
	for($r=0;$r<count($matrix_msg);$r++){
		for($c=0;$c<count($matrix_msg[$r]);$c++){
			if($i < strlen($message)){
				$matrix_msg[$c][$r]=hexdec(String2Hex($message[$i]));
				
				$i=$i+1;
			}
			if($j < strlen($cleachiffre)){
				$cle[$c][$r]=hexdec(String2Hex($cleachiffre[$j]));	
				$j=$j+1;
			}
		}
	}
}

echo '<table border="1" >';
echo '<tr>';
echo '<td></td>';
echo '<td width=120 align="center">Start of round</td>';
echo '<td width=120 align="center">After SubBytes</td>';
echo '<td width=120 align="center">After ShiftRows</td>';
echo '<td width=120 align="center">After MixColums</td>';
echo '<td width=120 align="center">Round key</td>';
echo '</tr><tr>';
echo '<td align="center">Input</td><td align="center">';
display($matrix_msg);
echo '</td><td></td><td></td><td></td>';
echo '<td align="center">';
display($cle);
echo '</td></tr>';

for($z=1;$z<=10;$z++) //rounds de 1 à 10
{
	echo '<tr><td align="center">Round '.$z.'</td><td align="center" >';  
	$res = array(array());

	echo '<table border="1">';
	for($r=0;$r<count($matrix_msg);$r++){
		echo '<tr>';
		for($c=0;$c<count($matrix_msg[$r]);$c++){
			$res[$r][$c]=$matrix_msg[$r][$c]^$cle[$r][$c];
			echo '<td align="center" >'.complete(dechex($res[$r][$c])).'</td>';
		}
	}

	echo '</table><td align="center">';
	$res=SubBytes($res);
	display($res);

	echo '</td><td align="center">';
	$res=shiftRows($res);
	display($res);

	echo '</td><td align="center">';
	if ($z<10){
		$res=mixColumns($res);
		$matrix_msg=$res;
		display($res);
	}
	echo '</td><td align="center">';

	$cle=keyExpansion($cle,$z); //round n
	display($cle);
	echo '</td></tr>';
}

echo '<tr><td>Output</td><td align="center"><table border="1" >';

for($r=0;$r<count($matrix_msg);$r++){
	echo '<tr>';
	for($c=0;$c<count($matrix_msg[$r]);$c++){
		$res[$r][$c]=$res[$r][$c]^$cle[$r][$c];
		echo '<td>'.complete(dechex($res[$r][$c])).'</td>';
	}
}
echo '</table></td></tr><tr><td></td><td align="center">Ciphertext</td></tr></table>';
?>
</html>