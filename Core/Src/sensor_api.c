#include <i2c.h>
#include <sensor_api.h>

// Acc  = prescurtare de la accelerometru
// Gyro = prescurtare de la giroscop

void initGyroAcc() {
	uint8_t cmd[1];// comanda registriilor
	cmd[0]=0x00;// Activity treshold disabled
	HAL_I2C_Mem_Write(&hi2c3, IMU_ADD, ACT_THS, 1, cmd,1,0x10);

	cmd[0]=0x00;// Activity duration disabled
	HAL_I2C_Mem_Write(&hi2c3, IMU_ADD, ACT_DUR, 1, cmd,1,0x10);

	cmd[0]=0x00;// Acc interrupt generator configure disabled
	HAL_I2C_Mem_Write(&hi2c3, IMU_ADD, INT_GEN_CFG_XL, 1, cmd,1,0x10);

	cmd[0]=0x00;// Acc interrupt treshold for x axis disabled
	HAL_I2C_Mem_Write(&hi2c3, IMU_ADD, INT_GEN_THS_X_XL, 1, cmd,1,0x10);

	cmd[0]=0x00;// Acc interrupt treshold for y axis disabled
	HAL_I2C_Mem_Write(&hi2c3, IMU_ADD, INT_GEN_THS_Y_XL, 1, cmd,1,0x10);

	cmd[0]=0x00;// Acc interrupt treshold for z axis disabled
	HAL_I2C_Mem_Write(&hi2c3, IMU_ADD, INT_GEN_THS_Z_XL, 1, cmd,1,0x10);

	cmd[0]=0x00;// Acc interrupt duration disabled
	HAL_I2C_Mem_Write(&hi2c3, IMU_ADD, INT_GEN_DUR_XL, 1, cmd,1,0x10);

	cmd[0]=0x00;// Gyro reference zero
	HAL_I2C_Mem_Write(&hi2c3, IMU_ADD, REFERENCE_G, 1, cmd,1,0x10);

	cmd[0]=0x00;// Acc interupt control 1 disabled
	HAL_I2C_Mem_Write(&hi2c3, IMU_ADD, INT1_CTRL, 1, cmd,1,0x10);

	cmd[0]=0x00;// Acc interrupt control 2 disabled
	HAL_I2C_Mem_Write(&hi2c3, IMU_ADD, INT2_CTRL, 1, cmd,1,0x10);

	cmd[0]=0x68;// Acc & Gyro ID register
	HAL_I2C_Mem_Write(&hi2c3, IMU_ADD, WHO_AM_I_XG, 1, cmd,1,0x10);

	cmd[0]=0xC0; // ODR - 952hz | DPS - 245
	HAL_I2C_Mem_Write(&hi2c3, IMU_ADD, CTRL_REG1_G, 1, cmd,1,0x10);

	cmd[0]=0x00;// Interrup and Out selection configure default
	HAL_I2C_Mem_Write(&hi2c3, IMU_ADD, CTRL_REG2_G, 1, cmd,1,0x10);

	cmd[0]=0x00;// Gyro Low Power, High-pass filter disabled and cutoff frequency default
	HAL_I2C_Mem_Write(&hi2c3, IMU_ADD, CTRL_REG3_G, 1, cmd,1,0x10);

	cmd[0]=0x00;// Gyro positive sign for pitch , roll , yaw axis
	HAL_I2C_Mem_Write(&hi2c3, IMU_ADD, ORIENT_CFG_G, 1, cmd,1,0x10);

	cmd[0]=0x00;// Gyro interupt source disabled
	HAL_I2C_Mem_Write(&hi2c3, IMU_ADD, INT_GEN_SRC_G, 1, cmd,1,0x10);

	cmd[0]=0x38;// Gyro Enable Axis : x , y , z
	HAL_I2C_Mem_Write(&hi2c3, IMU_ADD, CTRL_REG4, 1, cmd,1,0x10);

	cmd[0]=0x38; // No Decimation, Acc Enable Axis: x y z
	HAL_I2C_Mem_Write(&hi2c3, IMU_ADD, CTRL_REG5_XL, 1, cmd,1,0x10);

	cmd[0]=0x20; // ODR 10hz | Bandwidth Selection: 408hz | Scale +- 2g
	HAL_I2C_Mem_Write(&hi2c3, IMU_ADD, CTRL_REG6_XL, 1, cmd,1,0x10);

	cmd[0]=0x00;// Acc high resolution disabled | filter bypassed
	HAL_I2C_Mem_Write(&hi2c3, IMU_ADD, CTRL_REG7_XL, 1, cmd,1,0x10);

	cmd[0]=0x04;// Auto-increment address | folosit pt functia de readValues()
	HAL_I2C_Mem_Write(&hi2c3, IMU_ADD, CTRL_REG8, 1, cmd,1,0x10);

	cmd[0]=0x00;// I2C enable
	HAL_I2C_Mem_Write(&hi2c3, IMU_ADD, CTRL_REG9, 1, cmd,1,0x10);

	cmd[0]=0x00;// Self test disabled
	HAL_I2C_Mem_Write(&hi2c3, IMU_ADD, CTRL_REG10, 1, cmd,1,0x10);

	cmd[0]=0x00;// Acc interrupt source disabled
	HAL_I2C_Mem_Write(&hi2c3, IMU_ADD, INT_GEN_SRC_XL, 1, cmd,1,0x10);

	cmd[0]=0x00;// Bypass mode, FIFO off
	HAL_I2C_Mem_Write(&hi2c3, IMU_ADD, FIFO_CTRL, 1, cmd,1,0x10);

	cmd[0]=0x00;// FIFO empty
	HAL_I2C_Mem_Write(&hi2c3, IMU_ADD, FIFO_SRC, 1, cmd,1,0x10);

	cmd[0]=0x00;// Gyro interrupt generator disabled
	HAL_I2C_Mem_Write(&hi2c3, IMU_ADD, INT_GEN_CFG_G, 1, cmd,1,0x10);

	cmd[0]=0x00;// Gyro interrupt treshold for high x axis disabled
	HAL_I2C_Mem_Write(&hi2c3, IMU_ADD, INT_GEN_THS_XH_G, 1, cmd,1,0x10);

	cmd[0]=0x00;// Gyro interrupt treshold for low x axis disabled
	HAL_I2C_Mem_Write(&hi2c3, IMU_ADD, INT_GEN_THS_XL_G, 1, cmd,1,0x10);

	cmd[0]=0x00;// Gyro interrupt treshold for high y axis disabled
	HAL_I2C_Mem_Write(&hi2c3, IMU_ADD, INT_GEN_THS_YH_G, 1, cmd,1,0x10);

	cmd[0]=0x00;// Gyro interrupt treshold for low y axis disabled
	HAL_I2C_Mem_Write(&hi2c3, IMU_ADD, INT_GEN_THS_YL_G, 1, cmd,1,0x10);

	cmd[0]=0x00;// Gyro interrupt treshold for high z axis disabled
	HAL_I2C_Mem_Write(&hi2c3, IMU_ADD, INT_GEN_THS_ZH_G, 1, cmd,1,0x10);

	cmd[0]=0x00;// Gyro interrupt treshold for low z axis disabled
	HAL_I2C_Mem_Write(&hi2c3, IMU_ADD, INT_GEN_THS_ZL_G, 1, cmd,1,0x10);

	cmd[0]=0x00;// Gyro interrupt generator duration disabled
	HAL_I2C_Mem_Write(&hi2c3, IMU_ADD, INT_GEN_DUR_G, 1, cmd,1,0x10);
}

void readGyroValues(double buff[3]) {
	uint8_t data[6];
	int16_t gyro_data[3];

	HAL_I2C_Mem_Read(&hi2c3, IMU_ADD, OUT_X_L_G, 1, data,6,0x10);

	for(uint8_t i = 0; i < 3; i++){
		uint8_t low = data[2 * i];
		uint8_t high = data[2 * i + 1];

		gyro_data[i] = ((high << 8) | low);// Post 8 shifts => HIGH 00000000 + 00000000 LOW = HIGH LOW 16bit result

		buff[i] = (double) gyro_data[i] * 8.75 * 0.001;// ( High + Low ) * scale const * 10^(-3) measured in dps
	}
}

void readAccValues(double buff[3]) {
	uint8_t data[6];
	int16_t acc_data[3];

	HAL_I2C_Mem_Read(&hi2c3, IMU_ADD, OUT_X_L_XL, 1, data,6,0x10);

	for(int i=0;i<3;i++){
		uint8_t low = data[2 * i];
		uint8_t high = data[2 * i + 1];
		acc_data[i] = ((high << 8) | low);// Post 8 shifts => HIGH 00000000 + 00000000 LOW = HIGH LOW 16bit result

		buff[i] = (double) acc_data[i] * 0.061 * 0.001;// ( High + Low ) * scale const * 10^(-3) measured in g
	}
}

//void MX_I2C3_Init(void)
//{
//	/* USER CODE BEGIN I2C3_Init 0 */
//
//	/* USER CODE END I2C3_Init 0 */
//
//	/* USER CODE BEGIN I2C3_Init 1 */
//
//	/* USER CODE END I2C3_Init 1 */
//	hi2c3.Instance = I2C3;
//	hi2c3.Init.ClockSpeed = 100000;
//	hi2c3.Init.DutyCycle = I2C_DUTYCYCLE_2;
//	hi2c3.Init.OwnAddress1 = 0;
//	hi2c3.Init.AddressingMode = I2C_ADDRESSINGMODE_7BIT;
//	hi2c3.Init.DualAddressMode = I2C_DUALADDRESS_DISABLE;
//	hi2c3.Init.OwnAddress2 = 0;
//	hi2c3.Init.GeneralCallMode = I2C_GENERALCALL_DISABLE;
//	hi2c3.Init.NoStretchMode = I2C_NOSTRETCH_DISABLE;
//	if (HAL_I2C_Init(&hi2c3) != HAL_OK)
//	{
//		Error_Handler();
//	}
//	/** Configure Analogue filter
//	 */
//	if (HAL_I2CEx_ConfigAnalogFilter(&hi2c3, I2C_ANALOGFILTER_ENABLE) != HAL_OK)
//	{
//		Error_Handler();
//	}
//	/** Configure Digital filter
//	 */
//	if (HAL_I2CEx_ConfigDigitalFilter(&hi2c3, 0) != HAL_OK)
//	{
//		Error_Handler();
//	}
//	/* USER CODE BEGIN I2C3_Init 2 */
//
//	/* USER CODE END I2C3_Init 2 */
//
//}





